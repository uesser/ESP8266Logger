#include "ESP8266Logger.h"

//_________________________________________________________________________
// PRIVATE METHODS
//_________________________________________________________________________

String ESP8266Logger::replaceURL(String url) {
  url.replace("%" , "%25");
  url.replace(" " , "%20");
  url.replace("!" , "%21");
  url.replace("\"", "%22");
  url.replace("#" , "%23");
  url.replace("$" , "%24");
  url.replace("&" , "%26");
  url.replace("'" , "%27");
  url.replace("(" , "%28");
  url.replace(")" , "%29");
  url.replace("*" , "%2A");
  url.replace("+" , "%2B");
  url.replace("," , "%2C");
  url.replace("-" , "%2D");
  url.replace("." , "%2E");
  url.replace("/" , "%2F");
  url.replace(":" , "%3A");
  url.replace(";" , "%3B");
  url.replace("<" , "%3C");
  url.replace("=" , "%3D");
  url.replace(">" , "%3E");
  url.replace("?" , "%3F");
  url.replace("@" , "%40");
  url.replace("[" , "%5B");
  url.replace("\\", "%5C");
  url.replace("]" , "%5D");
  url.replace("{" , "%7B");
  url.replace("|" , "%7C");
  url.replace("}" , "%7D");
  
  return url;
}

//_________________________________________________________________________
// PUBLIC METHODS
//_________________________________________________________________________

ESP8266Logger::ESP8266Logger()
{
#ifndef ENERGY_EFFICIENT  
  _logDestList = LinkedList<LogDest>();
#endif
}

ESP8266Logger::~ESP8266Logger() {
#ifndef ENERGY_EFFICIENT  
  for (int i = 0; i < _logDestList.size(); i++) {
    if (_logDestList.get(i).logClient != 0) {
      if (_logDestList.get(i).logClient->connected()) {
        _logDestList.get(i).logClient->flush();
        _logDestList.get(i).logClient->stop();
      }
      delete _logDestList.get(i).logClient;
    }
  }
  _logDestList.clear();
#endif
}

int ESP8266Logger::regLogDestSerial(LogLevel logLev, LogSerial logSer) {
#ifndef ENERGY_EFFICIENT  
  if (_logDestList.size() < MAX_LOG_DEST_ENTRIES) {
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
#endif
}

int ESP8266Logger::regLogDestWifi(LogLevel logLev, String logHost, String logPort, String logURL,
                                  String logFileName, String logLevelParam, String logFunctionParam,
                                  String logStrParam, String logStrlnParam) {
#ifndef ENERGY_EFFICIENT  
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
#endif
}

void ESP8266Logger::unregLogDestSerial(LogSerial logSer) {
#ifndef ENERGY_EFFICIENT  
  int i = 0;
  
  while (i < _logDestList.size()) {
    if (_logDestList.get(i).logSerial == logSer) {
      _logDestList.remove(i);
    }
    
    i++;
  }
#endif
}

void ESP8266Logger::unregLogDestWifi(String logHost, String logPort) {
#ifndef ENERGY_EFFICIENT  
  int i = 0;
  
  while (i < _logDestList.size()) {
    if (_logDestList.get(i).logHost == logHost && _logDestList.get(i).logPort == logPort) {
      _logDestList.remove(i);
    }
    
    i++;
  }
#endif
}

void ESP8266Logger::log(LogLevel logLev, String logFct, String logStr, boolean prtln, int logDestIdx) {
#ifndef ENERGY_EFFICIENT  
  int     i        = 0;
  boolean replaced = false;
  
  static String sURL;

  if (logDestIdx >= 0)
    i = logDestIdx;
  
  while (i < _logDestList.size()) {
    if (logLev >= _logDestList.get(i).logLevel) {
      if (_logDestList.get(i).logSerial == LOG_SERIAL) {
        if (prtln) {
          if (logStr.length() > 0) {
            Serial.println(String(_logLevelStr[logLev]) + " - " + logFct + " - " + logStr);
          }
          else {
            Serial.println();
          }
        }
        else {
          if (logStr.length() > 0) {
            Serial.print(String(_logLevelStr[logLev]) + " - " + logFct + " - " + logStr);
          }
        }
      }
      if (_logDestList.get(i).logSerial == LOG_SERIAL1) {
        if (prtln) {
          if (logStr.length() > 0) {
            Serial1.println(String(_logLevelStr[logLev]) + " - " + logFct + " - " + logStr);
          }
          else {
            Serial1.println();
          }
        }
        else {
          if (logStr.length() > 0) {
            Serial1.print(String(_logLevelStr[logLev]) + " - " + logFct + " - " + logStr);
          }
        }
      }

      if (_logDestList.get(i).logClient != 0) {
  	    if (! _logDestList.get(i).logClient->connected()) {
          _logDestList.get(i).logClient->connect(_logDestList.get(i).logHost.c_str(), _logDestList.get(i).logPort.toInt());
  //        _logDestList.get(i).logClient->setNoDelay(true);
        }
        if (_logDestList.get(i).logClient->connected()) {
					sURL = String("GET ") + _logDestList.get(i).logURL + "?" +
							 "logFile=" + replaceURL(_logDestList.get(i).logFileName) + "&" +
							 _logDestList.get(i).logLevelParam + "=" + String(_logLevelStr[logLev]) + "&" +
							 _logDestList.get(i).logFunctionParam + "=" + replaceURL(logFct);

          if (logStr.length() > 0) {
            sURL += "&" + 
                    ((prtln) ? _logDestList.get(i).logStrlnParam : _logDestList.get(i).logStrParam) + "=" + replaceURL(logStr) +
							      " HTTP/1.1\r\nHost: " + _logDestList.get(i).logHost + ":" + _logDestList.get(i).logPort +
							      "\r\nConnection: Keep-Alive\r\n\r\n";
          }
          else {
            sURL += " HTTP/1.1\r\nHost: " + _logDestList.get(i).logHost + ":" + _logDestList.get(i).logPort +
							      "\r\nConnection: Keep-Alive\r\n\r\n";
          }

//          Serial.print("replaced URL: ");
//          Serial.println(sURL);
          
					_logDestList.get(i).logClient->print(sURL);
          _logDestList.get(i).logClient->flush();
        }
      }
    }

    if (logDestIdx >= 0)
      i = _logDestList.size() + 1;
    else
      i++;
  }
#endif
}
