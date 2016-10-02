a = [1,2,3];
Array.toString();
a.toString();

a = true;
Boolean.toString();
a.toString();

a = 34;
Number.toString();
a.toString();

a = "Hello";
String.toString();
a.toString();

a = Date(1000);
Date.toString();
a.toString();

a = null;
try { a.toString(); } catch (e) { }

a = undefined;
try { a.toString(); } catch (e) { }
