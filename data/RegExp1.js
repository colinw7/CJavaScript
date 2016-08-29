Object.getOwnPropertyNames(RegExp);

a = RegExp("ab+c");

console.log(a);

var re = /(\w+)\s(\w+)/;
var str = 'John Smith';
var newstr = str.replace(re, '$2, $1');
console.log(newstr);

var text = 'Some text\nAnd some more\r\nAnd yet\rThis is the end';
var lines = text.split(/\r\n|\r|\n/);
console.log(lines); // logs [ 'Some text', 'And some more', 'And yet', 'This is the end' ]

var s = 'Please yes\nmake my day!';
s.match(/yes.*day/);
// Returns null
s.match(/yes[^]*day/);
// Returns ["yes\nmake my day"]

//var str = '#foo#';
//var regex = /foo/y;

//regex.lastIndex = 1;
//regex.test(str); // true
//regex.lastIndex = 5;
//regex.test(str); // false (lastIndex is taken into account with sticky flag)
//regex.lastIndex; // 0 (reset after match failure)

var text = 'Образец text на русском языке';
var regex = /[\u0400-\u04FF]+/g;

var match = regex.exec(text);
console.log(match[0]);        // logs 'Образец'
console.log(regex.lastIndex); // logs '7'

var match2 = regex.exec(text);
console.log(match2[0]);       // logs 'на' [did not log 'text']
console.log(regex.lastIndex); // logs '15'

// and so on

var url = 'http://xxx.domain.com';
console.log(/[^.]+/.exec(url)[0].substr(7)); // logs 'xxx'
