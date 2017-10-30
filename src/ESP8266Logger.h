#ifndef __ESP8266LOGGER__
#define __ESP8266LOGGER__

#include <Arduino.h>
#include <ESP8266WiFi.h>

enum LogSerial { LOG_UNDEF, LOG_SERIAL, LOG_SERIAL1 };

enum   LogLevel      {  DEBUG,   INFO,   WARNING,   ERROR,   FATAL  };
String logLevelStr[] { "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };

typedef struct {
  LogSerial    logSerial;
  WiFiClient * logClient;
  String       logHost;
  String       logPort;
  String       logURL;
  String       logFileName;
  String       logLevelParam;
  String       logFunctionParam;
  String       logStrParam;
  String       logStrlnParam;
  LogLevel     logLevel;
} LogDest;

#define MAX_LOG_DEST_ENTRIES 10;

class Logger {
  public:
    ESP8266Logger();
    ~ESP8266Logger();

    int regLogDestSerial(LogLevel logLev, LogSerial logSer);
    int regLogDestWifi(LogLevel logLev, String logHost, String logPort, String logURL,
                       String logFileName, String logLevelParam, String logFunctionParam,
                       String logStrParam, String logStrlnParam);

    void log(LogLevel logLev, String logFct, String logStr, boolean prtln = false);
    void log(int logDestIdx, LogLevel logLev, String logFct, String logStr, boolean prtln = false);

    void debug(LogLevel logLev, String logFct, String logStr)                   { log(DEBUG, logFct, logStr, false); }
    void debug(int logDestIdx, LogLevel logLev, String logFct, String logStr)   { log(logDestIdx, DEBUG, logFct, logStr, false); }
    void info(LogLevel logLev, String logFct, String logStr)                    { log(INFO, logFct, logStr, false); }
    void info(int logDestIdx, LogLevel logLev, String logFct, String logStr)    { log(logDestIdx, INFO, logFct, logStr, false); }
    void warn(LogLevel logLev, String logFct, String logStr)                    { log(WARN, logFct, logStr, false); }
    void warn(int logDestIdx, LogLevel logLev, String logFct, String logStr)    { log(logDestIdx, WARN, logFct, logStr, false); }
    void error(LogLevel logLev, String logFct, String logStr)                   { log(ERROR, logFct, logStr, false); }
    void error(int logDestIdx, LogLevel logLev, String logFct, String logStr)   { log(logDestIdx, ERROR, logFct, logStr, false); }
    void fatal(LogLevel logLev, String logFct, String logStr)                   { log(FATAL, logFct, logStr, false); }
    void fatal(int logDestIdx, LogLevel logLev, String logFct, String logStr)   { log(logDestIdx, FATAL, logFct, logStr, false); }

    void debugln(LogLevel logLev, String logFct, String logStr)                 { log(DEBUG, logFct, logStr, true); }
    void debugln(int logDestIdx, LogLevel logLev, String logFct, String logStr) { log(logDestIdx, DEBUG, logFct, logStr, true); }
    void infoln(LogLevel logLev, String logFct, String logStr)                  { log(INFO, logFct, logStr, true); }
    void infoln(int logDestIdx, LogLevel logLev, String logFct, String logStr)  { log(logDestIdx, INFO, logFct, logStr, true); }
    void warnln(LogLevel logLev, String logFct, String logStr)                  { log(WARN, logFct, logStr, true); }
    void warnln(int logDestIdx, LogLevel logLev, String logFct, String logStr)  { log(logDestIdx, WARN, logFct, logStr, true); }
    void errorln(LogLevel logLev, String logFct, String logStr)                 { log(ERROR, logFct, logStr, true); }
    void errorln(int logDestIdx, LogLevel logLev, String logFct, String logStr) { log(logDestIdx, ERROR, logFct, logStr, true); }
    void fatalln(LogLevel logLev, String logFct, String logStr)                 { log(FATAL, logFct, logStr, true); }
    void fatalln(int logDestIdx, LogLevel logLev, String logFct, String logStr) { log(logDestIdx, FATAL, logFct, logStr, true); }

    void logln(LogLevel logLev, String logFct, String logStr)                   { log(logLev, logFct, logStr, true); }
    void logln(int logDestIdx, LogLevel logLev, String logFct, String logStr)   { log(logDestIdx, logLev, logFct, logStr, true); }

  protected:
    LinkedList<LogDest> _logDestList;
}

#endif  // __ESP8266LOGGER__
