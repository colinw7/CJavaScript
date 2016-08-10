width  = 400;
height = 400;

var numBalls = 100 ;
var maxSize  = 8;
var minSize  = 5;
var maxSpeed = maxSize+5;

tempRadius = Math.floor(Math.random()*maxSize)+minSize;

tempX       = tempRadius*2 + (Math.floor(Math.random()*width )-tempRadius*2);
tempY       = tempRadius*2 + (Math.floor(Math.random()*height)-tempRadius*2);
tempSpeed   = maxSpeed-tempRadius;
tempAngle   = Math.floor(Math.random()*360);
tempRadians = tempAngle * Math.PI/ 180;
tempXunits  = Math.cos(tempRadians) * tempSpeed;
tempYunits  = Math.sin(tempRadians) * tempSpeed;

tempBall = {x:tempX,y:tempY,radius:tempRadius, speed:tempSpeed, angle:tempAngle, xunits:tempXunits, yunits:tempYunits};

console.log(tempBall);
