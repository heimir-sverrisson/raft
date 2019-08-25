#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <raft_util.h>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

void log_init() {
    logging::register_simple_formatter_factory<
        boost::log::trivial::severity_level, char>("Severity");
    logging::add_file_log(
        keywords::file_name = "raft_%N.log",
        keywords::rotation_size = 50 * 1024 * 1024,
        keywords::time_based_rotation =
            sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::auto_flush = true,
        keywords::format = "[%TimeStamp%]:\t%Severity%\t%ThreadID%\t%Message%");

    logging::core::get()->set_filter(logging::trivial::severity >=
                                     logging::trivial::info);
    logging::add_common_attributes();
}

typedef sinks::synchronous_sink<sinks::syslog_backend> sink_t;

void log_syslog_init() {
    boost::shared_ptr<logging::core> core = logging::core::get();

    // Create a new backend
    boost::shared_ptr<sinks::syslog_backend> backend(new sinks::syslog_backend(
        keywords::facility = sinks::syslog::local0,
        keywords::use_impl = sinks::syslog::udp_socket_based));

    // Setup the target address and port to send syslog messages to
    backend->set_target_address("127.0.0.1", 514);

    // Create and fill in another level translator for "MyLevel" attribute of
    // type string
    sinks::syslog::custom_severity_mapping<std::string> mapping("MyLevel");
    mapping["debug"] = sinks::syslog::debug;
    mapping["normal"] = sinks::syslog::info;
    mapping["warning"] = sinks::syslog::warning;
    mapping["failure"] = sinks::syslog::critical;
    backend->set_severity_mapper(mapping);

    // Wrap it into the frontend and register in the core.
    core->add_sink(boost::make_shared<sink_t>(backend));
}
