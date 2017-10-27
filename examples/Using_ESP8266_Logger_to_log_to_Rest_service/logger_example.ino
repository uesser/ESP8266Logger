include <Logger.h>

#define LOG_HOST 123.123.123.123
#define LOG_PORT 3000

Logger log;

void setup() {
  String logFct = "SETUP";

  // register some log destinations (Serial, Serial1 and a Restservice)
  int log2Serial = regLogDestSerial(DEBUG, LOG_SERIAL);
  regLogDestSerial(FATAL, LOG_SERIAL1);
  regLogDestWifi(INFO, LOG_HOST, LOG_PORT, "/log", "logLev", "logFct", "logStr", "logStrln");

  // will be logged to Serial, but not to Serial1 and Restservice because Serial is the only one defined for logging DEBUG info
  log.debug(logFct, "some debug info");

  // will be logged to Serial and Restservice, but not to Serial1 because Serial1 defined for logging FATAL only
  log.warn(logFct, "some warning info");

  // will be logged to Serial only because of the 1. parameter (log2Serial)
  log.info(log2Serial, logFct, "some info logging");

  // will be logged to all
  log.fatal(logFct, "some fatal error");

  // will be logged to Serial and Restservice, but not to Serial1 because Serial1 defined for logging FATAL only
  log.error(logFct, "some error logging");
}

void loop() {
  String logFct = "LOOP";

  // will be logged to Serial and Restservice, but not to Serial1 because Serial1 defined for logging FATAL only
  log.info(logFct, "some info logging");
}
