#ifndef __LOGGER__
#define __LOGGER__

#include <Arduino.h>
#include <ESP8266WiFi.h>

enum LogSerial { LOG_UNDEF, LOG_SERIAL, LOG_SERIAL1 };

enum   LogLevel      {  DEBUG,   INFO,   WARNING,   ERROR,   FATAL  };
String logLevelStr[] { "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };

typedef struct {
  LogSerial    logSerial;
  WifiClient * logClient;
  String       logHost;
  String       logPort;
  String       logURL;
  String       logLevelParam;
  String       logFunctionParam;
  String       logStrParam;
  String       logStrlnParam;
  LogLevel     logLevel;
} LogDest;

#define MAX_LOG_DEST_ENTRIES 10;

#define logln(logLev, logFct, logStr) log(logLev, logFct, logStr, true)
#define logln(logDestIdx, logLev, logFct, logStr) log(logDestIdx, logLev, logFct, logStr, true)

class Logger {
  public:
    Logger();
    ~Logger();

    int regLogDestSerial(LogLevel logLev, LogSerial logSer);
    int regLogDestWifi(LogLevel logLev, String logHost, String logPort, String logURL,
                       String logLevelParam, String logFunctionParam, String logStrParam,
                       String logStrlnParam);

    void log(LogLevel logLev, String logFct, String logStr, boolean prtln = false);
    void log(int logDestIdx, LogLevel logLev, String logFct, String logStr, boolean prtln = false);

  protected:
    LinkedList<LogDest> _logDestList;
}

#endif  // __LOGGER__
