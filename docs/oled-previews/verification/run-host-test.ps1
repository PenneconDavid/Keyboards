$ErrorActionPreference = 'Stop'
$compiler = 'C:\Program Files\QMK_MSYS\mingw64\bin\gcc.exe'
$env:PATH = 'C:\Program Files\QMK_MSYS\mingw64\bin;C:\Program Files\QMK_MSYS\usr\bin;' + $env:PATH
& $compiler -std=c11 -Wall -Wextra -Werror -I $PSScriptRoot (Join-Path $PSScriptRoot 'oled_host_test.c') -o (Join-Path $PSScriptRoot 'oled_host_test.exe')
if ($LASTEXITCODE -ne 0) { throw "Host test compile failed: $LASTEXITCODE" }
& (Join-Path $PSScriptRoot 'oled_host_test.exe')
if ($LASTEXITCODE -ne 0) { throw "Host tests failed: $LASTEXITCODE" }
