#ifndef Config_h
#define Config_h

// Static (compile time) configuration

using namespace std;

class Config{
  public:
    const static int readPeriod = 100; // Maxium read period for Receiver (msec)
    const static int maxMessageSize = 1000; // In bytes
    const static char messageSeparator = '|';  // Separating type and payload
};
#endif
