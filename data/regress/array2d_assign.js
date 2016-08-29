var nodes = [];

for (var y = 0; y < 2; y++) {
  nodes[y] = [];

  for (var x = 0; x < 2; x++) {
    nodes[y][x] = y*2 + x;
  }
}

console.log(nodes);
