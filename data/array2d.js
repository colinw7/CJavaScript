var tileMap = [
  [32,31,31,31,1,31,31,31,31,32]
  , [1,1,1,1,1,1,1,1,1,1]
  , [32,1,26,1,26,1,26,1,1,32]
  , [32,26,1,1,26,1,1,26,1,32]
  , [32,1,1,1,26,26,1,26,1,32]
  , [32,1,1,26,1,1,1,26,1,32]
  , [32,1,1,1,1,1,1,26,1,32]
  , [1,1,26,1,26,1,26,1,1,1]
  , [32,1,1,1,1,1,1,1,1,32]
  , [32,31,31,31,1,31,31,31,31,32]
  ];
var mapIndexOffset = -1;
var rowCtr=0;
var colCtr=0;
var tileId = tileMap[rowCtr][colCtr]+mapIndexOffset;
console.log(rowCtr);
console.log(colCtr);
console.log(tileId);
