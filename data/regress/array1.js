var myArray = []; // Point the variable myArray to a newly created, empty Array
console.log(myArray);

myArray.push("hello World"); // Fill the next empty index, in this case 0

console.log(myArray);
console.log(myArray[0]);     // Equivalent to alert("hello World");

console.log(myArray[1]); // the 2nd item in myArray
console.log(myArray["1"]);

console.log(myArray);

myArray = [0, 1,, , 4, 5]; // array with length 6 and 6 elements, including 2 undefined elements
console.log(myArray);

myArray = new Array(0, 1, 2, 3, 4, 5); // array with length 6 and 6 elements
console.log(myArray);

myArray = new Array(10); // an empty array with length 10
console.log(myArray);

console.log(myArray.length);
