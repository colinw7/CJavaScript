function FrameRateCounter() {
  this.lastFrameCount=0;
  var dateTemp =new Date();
  this.frameLast=dateTemp.getTime();
  delete dateTemp;
  this.frameCtr=0;
}

FrameRateCounter();

console.log(this.lastFrameCount);
console.log(lastFrameCount);
console.log(this.frameLast);
console.log(frameLast);
console.log(this.frameCtr);
console.log(frameCtr);
