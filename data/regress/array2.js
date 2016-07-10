animals = ["cat", "dog", "tiger"]; // Initialization
console.log(animals);

fruits = [["apple", "orange"], ["pear"]]; // Initialization of a nested array
console.log(fruits);

cat = animals[0]; // Indexed access
console.log(cat);

apple = fruits[0][0]; // Indexed access, nested
console.log(apple);

animals[0] = "fox"; // Write indexed access
console.log(animals);

for (var i = 0; i < animals.length; ++i) {
  item = animals[i]; // For each loop
  console.log(item);
}

animals = animals.concat("mouse"); // Append
console.log(animals);

animals = animals.concat(["horse", "ox"]); // Expand
console.log(animals);

animals.pop(); // Yields "ox", removing it from animals
console.log(animals);

animals.push("ox"); // Push ox back to the end
console.log(animals);

animals.shift(); // Yields "fox", removing it from animals
console.log(animals);

animals.unshift("fox"); // Place fox back to the beginning
console.log(animals);

mytext = [1, 2, 3].join("-"); // Yields "1-2-3" via join
console.log(mytext);

items = mytext.split("-"); // Splits
console.log(items);
