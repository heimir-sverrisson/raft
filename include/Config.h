#ifndef Config_h
#define Config_h

// Static (compile time) configuration

using namespace std;

struct Config{
    const static int readPeriod = 500; // Maxium read period for Receiver (msec)
    const static int leaderPeriod = readPeriod/2; // Maxium period between AppendEntries from a leader
    const static int maxMessageSize = 1000; // In bytes
    const static char messageSeparator = '|';  // Separating type and payload
};
#endif
