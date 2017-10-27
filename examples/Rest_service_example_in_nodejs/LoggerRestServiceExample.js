var express = require('express');
var fs = require('fs');

var buffer = new Buffer();

var app = express();

app.get('/log', function(request, response) {
  if (request.query.logLev) {
    buffer = request.query.logLev;
  }
  else {
    buffer = "UNDEF";
  }
  if (request.query.logFct) {
    buffer += " - " + request.query.logFct;
  }
  if (request.query.logStr) {
    buffer += " - " + request.query.logStr;
  }
  if (request.query.logStrln) {
    buffer += " - " + request.query.logStrln + "\n";
  }

  fs.appendFileSync("/var/log/logfile.log", buffer);
});

app.listen(3000);

console.log("node express app started at http://localhost:3000");
