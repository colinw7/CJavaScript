console.log(Date());

d = new Date();

Date.UTC(2015, 12);
Date.now();
Date.parse("Dec 6 1975");

d = new Date(187074000000);

console.log(d);

console.log("getDate", d.getDate());
console.log("getDay", d.getDay());
console.log("getFullYear", d.getFullYear());
console.log("getHours", d.getHours());
console.log("getMilliseconds", d.getMilliseconds());
console.log("getMinutes", d.getMinutes());
console.log("getMonth", d.getMonth());
console.log("getSeconds", d.getSeconds());
console.log("getTime", d.getTime());
console.log("getTimezoneOffset", d.getTimezoneOffset());
console.log("getUTCDate", d.getUTCDate());
console.log("getUTCDay", d.getUTCDay());
console.log("getUTCFullYear", d.getUTCFullYear());
console.log("getUTCHours", d.getUTCHours());
console.log("getUTCMilliseconds", d.getUTCMilliseconds());
console.log("getUTCMinutes", d.getUTCMinutes());
console.log("getUTCMonth", d.getUTCMonth());
console.log("getUTCSeconds", d.getUTCSeconds());
console.log("getYear", d.getYear());

d.setDate(24); console.log(d.toString());
d.setFullYear(1965); console.log(d.toString());
d.setHours(11); console.log(d.toString());
d.setMilliseconds(5); console.log(d.toString());
d.setMinutes(45); console.log(d.toString());
d.setMonth(2); console.log(d.toString());
d.setSeconds(32); console.log(d.toString());
d.setTime(100); console.log(d.toString());
d.setUTCDate(24); console.log(d.toString());
d.setUTCFullYear(1965); console.log(d.toString());
d.setUTCHours(11); console.log(d.toString());
d.setUTCMilliseconds(5); console.log(d.toString());
d.setUTCMinutes(45); console.log(d.toString());
d.setUTCMonth(2); console.log(d.toString());
d.setUTCSeconds(32); console.log(d.toString());
d.setYear(1865); console.log(d.toString());

console.log("toDateString", d.toDateString());
console.log("toGMTString", d.toGMTString());
console.log("toISOString", d.toISOString());
console.log("toJSON", d.toJSON());
console.log("toLocalDateString", d.toLocaleDateString());
//console.log("toLocalFormat", d.toLocaleFormat());
console.log("toLocalFormat", d.toLocaleString());
console.log("toLocalTimeString", d.toLocaleTimeString());
//console.log("toSource", d.toSource());
console.log("toString", d.toString());
console.log("toTimeString", d.toTimeString());
console.log("toUTCString", d.toUTCString());
console.log("valueOf", d.valueOf());
