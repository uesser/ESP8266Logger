#include "ESP8266Logger.h"

ESP8266Logger::ESP8266Logger()
:_logDestList(0)
{
  _logDestList = LinkedList<LogDest>();
}

ESP8266Logger::~ESP8266Logger() {
  for (int i = 0; i < _logDestList.size(); i++) {
    if (_logDestList[i].logClient != 0) {
      if (_logDestList[i].logClient->connected()) {
        _logDestList[i].logClient->flush();
        _logDestList[i].logClient->close();
      }
      delete _logDestList[i].logClient;
    }
  }
  _logDestList.clear();
}

int ESP8266Logger::regLogDestSerial(LogLevel logLev, LogSerial logSer) {
  if (_logDestList.size() < MAX_LOG_DEST_ENTRIES {
    LogDest logDest;

    logDest.logClient = 0;
    logDest.logSerial = logSer;
    logDest.logLevel  = logLev;

    if (_logDestList.add(logDest)) {
      return _logDestList.size() - 1;
    }
    else {
      return -1;
    }
  }
  else {
      return -2;
  }
}

int ESP8266Logger::regLogDestWifi(LogLevel logLev, String logHost, String logPort, String logURL,
                                  String logFileName, String logLevelParam, String logFunctionParam,
                                  String logStrParam, String logStrlnParam) {
  if (_logDestList.size() < MAX_LOG_DEST_ENTRIES) {
    LogDest logDest;

    logDest.logSerial        = LOG_UNDEF;
    logDest.logClient        = new WiFiClient;
    logDest.logHost          = logHost;
    logDest.logPort          = logPort;
    logDest.logURL           = logURL;
    logDest.logFileName      = logFileName;
    logDest.logLevelParam    = logLevelParam;
    logDest.logFunctionParam = logFunctionParam;
    logDest.logStrParam      = logStrParam;
    logDest.logStrlnParam    = logStrlnParam;
    logDest.logLevel         = logLev;

    if (_logDestList.add(logDest)) {
      return _logDestList.size() - 1;
    }
    else {
      return -1;
    }
  }
  else {
      return -2;
  }
}

void ESP8266Logger::log(int logDestIdx, LogLevel logLev, String LogFct, String logStr, boolean prtln) {
  if (logDestIdx >= 0 && logDestIdx < _logDestList.size() &&
      (_logDestList[logDestIdx].logSerial != 0 || _logDestList[logDestIdx].logClient != 0) {
    log(logLev, logFct, logStr, prtln);
  }
}

void ESP8266Logger::log(LogLevel logLev, String LogFct, String logStr, boolean prtln) {
  int     i        = 0;
  boolean replaced = false;

  while (i < _logDestList.size()) {
    if (logLev >= _logDestList[i].logLevel) {
      if (_logDestList[i].logSerial == LOG_SERIAL) {
        if (prtln)
          Serial.println(logLev + " - " + logFct + " - " + logStr);
        else
          Serial.print(logLev + " - " + logFct + " - " + logStr);
      }
      if (_logDestList[i].logSerial == LOG_SERIAL1) {
        if (prtln)
          Serial1.println(logLev + " - " + logFct + " - " + logStr);
        else
          Serial1.print(logLev + " - " + logFct + " - " + logStr);
      }

      if (_logDestList[i].logClient != 0) {
        if (! _logDestList[i].logClient->connected()) {
          _logDestList[i].logClient->connect(_logDestList[i].logHost.c_str(), _logDestList[i].logPort.toInt());
  //        _logDestList[i].logClient->setNoDelay(true);
        }
        if (_logDestList[i].logClient->connected()) {
          if (! replaced) {
            String sURL = "GET " + _logDestList[i].logURL + "?" +
                          "logFile=" + _logDestList[i].logFileName + "&" +
                          _logDestList[i].logLevelParam + "=" + logLevelStr[logLev] + "&" +
                          _logDestList[i].logFunctionParam + "=" + logFct + "&" +
                          (prtln) ? _logDestList[i].logStrlnParam : _logDestList[i].logStrParam + "=" + logStr +
                          " HTTP/1.1\r\nHost: " + _logDestList[i].logHost + ":" + _logDestList[i].logPort +
                          "\r\nConnection: Keep-Alive\r\n\r\n";

            sURL.replace(" " , "%20");
            sURL.replace("!" , "%21");
            sURL.replace("\"", "%22");
            sURL.replace("#" , "%23");
            sURL.replace("$" , "%24");
            sURL.replace("%" , "%25");
            sURL.replace("&" , "%26");
            sURL.replace("'" , "%27");
            sURL.replace("(" , "%28");
            sURL.replace(")" , "%29");
            sURL.replace("*" , "%2A");
            sURL.replace("+" , "%2B");
            sURL.replace("," , "%2C");
            sURL.replace("-" , "%2D");
            sURL.replace("." , "%2E");
            sURL.replace("/" , "%2F");
            sURL.replace(":" , "%3A");
            sURL.replace(";" , "%3B");
            sURL.replace("<" , "%3C");
            sURL.replace("=" , "%3D");
            sURL.replace(">" , "%3E");
            sURL.replace("?" , "%3F");
            sURL.replace("@" , "%40");
            sURL.replace("[" , "%5B");
            sURL.replace("\\", "%5C");
            sURL.replace("]" , "%5D");
            sURL.replace("{" , "%7B");
            sURL.replace("|" , "%7C");
            sURL.replace("}" , "%7D");

            replaced = true;
          }

          _logDestList[i].logClient->print(sURL);
          _logDestList[i].logClient->flush();
        }
      }
    }

    i++;
  }
}
