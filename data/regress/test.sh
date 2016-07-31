#!/bin/csh -f

if ($#argv > 0) then
  foreach file ($argv)
    echo "=== $file ==="

    set res = $file:r.out

    CJavaScriptTest $file > output/$res

    if (! -e golden/$res) then
      nodejs $file > golden/$res
    endif

    diff output/$res golden/$res
  end
else
  foreach file (*.js)
    echo "=== $file ==="

    set res = $file:r.out

    CJavaScriptTest $file > output/$res

    if (! -e golden/$res) then
      nodejs $file > golden/$res
    endif

    diff output/$res golden/$res
  end
endif

exit 0
