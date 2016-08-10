canvas = document.getElementById("canvas");

ctx = canvas.getContext("2d");

w = canvas.width;
h = canvas.height;

ctx.fillStyle = '#999999';

ctx.fillRect(10, 10, 100, 100);

ctx.strokeStyle = '#FF0000';

ctx.moveTo(20, 20);
ctx.lineTo(80, 80);

ctx.stroke();
