#ifndef __ESP8266LOGGER__
#define __ESP8266LOGGER__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LinkedList.h>

enum LogSerial { LOG_UNDEF, LOG_SERIAL, LOG_SERIAL1 };
enum LogLevel  { DEBUG, INFO, WARNING, ERROR, FATAL  };

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

#define MAX_LOG_DEST_ENTRIES 10

class ESP8266Logger {
  public:
    ESP8266Logger();
    ~ESP8266Logger();

    int regLogDestSerial(LogLevel logLev, LogSerial logSer);
    int regLogDestWifi(LogLevel logLev, String logHost, String logPort, String logURL,
                       String logFileName, String logLevelParam, String logFunctionParam,
                       String logStrParam, String logStrlnParam);

    void unregLogDestSerial(LogSerial logSer);
    void unregLogDestWifi(String logHost, String logPort);

    void log(LogLevel logLev, String logFct, String logStr, boolean prtln = false, int logDestIdx = -1);

    void log(int logDestIdx, LogLevel logLev, String logFct, String logStr, boolean prtln) {
      log(logLev, logFct, logStr, prtln, logDestIdx);
    }

    void logln(LogLevel logLev, String logFct, String logStr)                 { log(logLev, logFct, logStr, true); }
    void logln(int logDestIdx, LogLevel logLev, String logFct, String logStr) { log(logDestIdx, logLev, logFct, logStr, true); }

    void debug(String logFct, String logStr)                                  { log(DEBUG, logFct, logStr, false); }
    void debug(int logDestIdx, String logFct, String logStr)                  { log(logDestIdx, DEBUG, logFct, logStr, false); }
    void info(String logFct, String logStr)                                   { log(INFO, logFct, logStr, false); }
    void info(int logDestIdx, String logFct, String logStr)                   { log(logDestIdx, INFO, logFct, logStr, false); }
    void warn(String logFct, String logStr)                                   { log(WARNING, logFct, logStr, false); }
    void warn(int logDestIdx, String logFct, String logStr)                   { log(logDestIdx, WARNING, logFct, logStr, false); }
    void error(String logFct, String logStr)                                  { log(ERROR, logFct, logStr, false); }
    void error(int logDestIdx, String logFct, String logStr)                  { log(logDestIdx, ERROR, logFct, logStr, false); }
    void fatal(String logFct, String logStr)                                  { log(FATAL, logFct, logStr, false); }
    void fatal(int logDestIdx, String logFct, String logStr)                  { log(logDestIdx, FATAL, logFct, logStr, false); }
                                                                              
    void debugln(String logFct, String logStr)                                { log(DEBUG, logFct, logStr, true); }
    void debugln(int logDestIdx, String logFct, String logStr)                { log(logDestIdx, DEBUG, logFct, logStr, true); }
    void infoln(String logFct, String logStr)                                 { log(INFO, logFct, logStr, true); }
    void infoln(int logDestIdx, String logFct, String logStr)                 { log(logDestIdx, INFO, logFct, logStr, true); }
    void warnln(String logFct, String logStr)                                 { log(WARNING, logFct, logStr, true); }
    void warnln(int logDestIdx, String logFct, String logStr)                 { log(logDestIdx, WARNING, logFct, logStr, true); }
    void errorln(String logFct, String logStr)                                { log(ERROR, logFct, logStr, true); }
    void errorln(int logDestIdx, String logFct, String logStr)                { log(logDestIdx, ERROR, logFct, logStr, true); }
    void fatalln(String logFct, String logStr)                                { log(FATAL, logFct, logStr, true); }
    void fatalln(int logDestIdx, String logFct, String logStr)                { log(logDestIdx, FATAL, logFct, logStr, true); }

  protected:
    LinkedList<LogDest> _logDestList;
    const char*         _logLevelStr[5] = { "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };
    
  private:
    String replaceURL(String url);
};

#endif  // __ESP8266LOGGER__
