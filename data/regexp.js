s = "JavaScript";

i = s.search(/script/i);

assert(i == 4);

s = "JAVASCRIPT";

s1 = s.replace(/java/gi, "Java");

assert(s1 == "JavaSCRIPT");

var quote = /"([^"]*)"/g

text = "\"a\""

text.replace(quote, '{$1}');
