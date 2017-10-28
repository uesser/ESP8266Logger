var express = require('express');
var fs = require('fs');

var buffer = new Buffer();

var app = express();

app.get('/log', function(request, response) {
  var now = new Date();
  var day = now.getDate();
  var month = now.getMonth() + 1;
  var year = now.getFullYear();
  var hour = now.getHours();
  var minute = now.getMinutes();
  var second = now.getSeconds();
  var day0  = ((day < 10) ? "0" : "");
  var month0  = ((month < 10) ? ".0" : "."); 
  var hour0  = ((hour < 10) ? "0" : ""); 
  var minute0  = ((minute < 10) ? ":0" : ":"); 
  var second0  = ((second < 10) ? ":0" : ":"); 
  var output = day0 + day + month0 + month + "." + year + " " + hour0 + hour + minute0 + minute + second0 + second;

  if (request.query.logLev) {
    output += " - " + request.query.logLev;
  }
  if (request.query.logFct) {
    output += " - " + request.query.logFct;
  }
  if (request.query.logStr) {
    output += " - " + request.query.logStr;
  }
  if (request.query.logStrln) {
    output += " - " + request.query.logStrln + "\n";
  }

//  console.log("req.query: %s", JSON.stringify(req.query));

  buffer = new Buffer.from(output);
  fs.appendFileSync("/var/log/logfile.log", buffer);

//  console.log("File written: %s", buffer);

  return res.send();
});

app.listen(3000);

console.log("node express app started at http://localhost:3000");
