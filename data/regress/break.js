for (var y = 0; y < 10; y++) {
  for (var x = 0; x < 10; x++) {
    if (x > 5 && y > 5) {
      break;
    }
    console.log("x="+x+" y="+y);
  }
}
