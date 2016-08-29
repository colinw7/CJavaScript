function ConsoleLog() {
}

//create function that will be added to the class
console_log=function(message) {
  if(typeof(console) !== 'undefined' && console != null) {
    console.log(message);
  }
};

ConsoleLog.log=console_log;
