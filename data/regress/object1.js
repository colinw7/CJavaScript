/**
 * Prints the alert message
 */
function message() {
  console.log(object_1.a + " " + object_2.a + " " + object_3.a);
}

/**
 * To learn JavaScript objects...
 */
var object_1 = {a: 1};   // assign reference of newly created object to object_1
var object_2 = {a: 0};
var object_3 = object_2; // object_3 references the same object as object_2 does
         
object_3.a = 2;
console.log(message()); // displays 1 2 2
         
object_2 = object_1; // object_2 now references the same object as object_1
                     // object_3 is now the only reference to what object_2 referenced before
console.log(message()); // displays 1 1 2
         
object_2.a = 7; // modifies object_1
console.log(message()); // displays 7 7 2
