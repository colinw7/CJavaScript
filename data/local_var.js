function f1() {
 var v1 = 'local value';

 console.log(v1);
 console.log(this.v1);

 console.log (v1 === 'local value');
 console.log (typeof(this.v1) === 'undefined');

 this.eval('var v1 = "global value";');

 console.log(v1);
 console.log(this.v1);

 console.log (v1 === 'local value');
 console.log (this.v1 === 'global value');
};

var v1 = "global value";

f1();
