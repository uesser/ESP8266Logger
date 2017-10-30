#ifndef __LOGGER__
#define __LOGGER__

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
  String       logLevelParam;
  String       logFunctionParam;
  String       logStrParam;
  String       logStrlnParam;
  LogLevel     logLevel;
} LogDest;

#define MAX_LOG_DEST_ENTRIES 10;

#define debug(logLev, logFct, logStr)               log(DEBUG, logFct, logStr, false)
#define debug(logDestIdx, logLev, logFct, logStr)   log(logDestIdx, DEBUG, logFct, logStr, false)
#define info(logLev, logFct, logStr)                log(INFO, logFct, logStr, false)
#define info(logDestIdx, logLev, logFct, logStr)    log(logDestIdx, INFO, logFct, logStr, false)
#define warn(logLev, logFct, logStr)                log(WARN, logFct, logStr, false)
#define warn(logDestIdx, logLev, logFct, logStr)    log(logDestIdx, WARN, logFct, logStr, false)
#define error(logLev, logFct, logStr)               log(ERROR, logFct, logStr, false)
#define error(logDestIdx, logLev, logFct, logStr)   log(logDestIdx, ERROR, logFct, logStr, false)
#define fatal(logLev, logFct, logStr)               log(FATAL, logFct, logStr, false)
#define fatal(logDestIdx, logLev, logFct, logStr)   log(logDestIdx, FATAL, logFct, logStr, false)

#define debugln(logLev, logFct, logStr)             log(DEBUG, logFct, logStr, true)
#define debugln(logDestIdx, logLev, logFct, logStr) log(logDestIdx, DEBUG, logFct, logStr, true)
#define infoln(logLev, logFct, logStr)              log(INFO, logFct, logStr, true)
#define infoln(logDestIdx, logLev, logFct, logStr)  log(logDestIdx, INFO, logFct, logStr, true)
#define warnln(logLev, logFct, logStr)              log(WARN, logFct, logStr, true)
#define warnln(logDestIdx, logLev, logFct, logStr)  log(logDestIdx, WARN, logFct, logStr, true)
#define errorln(logLev, logFct, logStr)             log(ERROR, logFct, logStr, true)
#define errorln(logDestIdx, logLev, logFct, logStr) log(logDestIdx, ERROR, logFct, logStr, true)
#define fatalln(logLev, logFct, logStr)             log(FATAL, logFct, logStr, true)
#define fatalln(logDestIdx, logLev, logFct, logStr) log(logDestIdx, FATAL, logFct, logStr, true)

#define logln(logLev, logFct, logStr)               log(logLev, logFct, logStr, true)
#define logln(logDestIdx, logLev, logFct, logStr)   log(logDestIdx, logLev, logFct, logStr, true)

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
