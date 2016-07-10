#!/bin/csh -f

foreach file (*.js)
  echo "=== $file ==="

  set res = $file:r.out

  CJavaScriptTest $file > output/$res

  if (! -e golden/$res) then
    nodejs $file > golden/$res
  endif

  diff output/$res golden/$res
end

exit 0
