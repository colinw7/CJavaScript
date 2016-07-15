function True () { console.log("true" ); return true ; }
function False() { console.log("false"); return false; }
function Run  () { console.log("run"  ); return true ; }

True();
False();

console.log(True () || Run());
console.log(False() || Run());

console.log(True () && Run());
console.log(False() && Run());
