dog = {color: "brown", size: "large"};
console.log(dog);

console.log(dog["color"]); // results in "brown"
console.log(dog.color);    // also results in "brown"

cats = [{color: "brown", size: "large"}, {color: "black", size: "small"}];
console.log(cats);

console.log(cats[0]["size"]); // results in "large"

dogs = {rover: {color: "brown", size: "large"}, spot: {color: "black", size: "small"}};
console.log(dogs);

console.log(dogs["spot"]["size"]); // results in "small"
console.log(dogs.rover.color);     // results in "brown"
