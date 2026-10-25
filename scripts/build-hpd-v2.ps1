<# Build only. This script never invokes a flash command or accesses a keyboard. #>
[CmdletBinding()]
param([string]$UpstreamPath = (Join-Path $PSScriptRoot '..\.build\hpd-v2\vial-qmk'))
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path $PSScriptRoot -Parent
$upstreamRoot = (Resolve-Path -LiteralPath $UpstreamPath).Path
$pinnedRevision = '98bf33054c73619b590505853d8d8f3e0557f41e'
$actualRevision = & git -C $upstreamRoot rev-parse HEAD
if ($LASTEXITCODE -ne 0 -or $actualRevision -ne $pinnedRevision) {
    throw "Expected Ergohaven source revision $pinnedRevision at $upstreamRoot"
}
$env:MSYSTEM = 'MINGW64'
$env:SHELL = 'C:\Program Files\QMK_MSYS\usr\bin\bash.exe'
$env:PYTHONUTF8 = '1'
$env:PATH = "C:\Program Files\QMK_MSYS\mingw64\bin;C:\Program Files\QMK_MSYS\usr\bin;$env:PATH"
& python (Join-Path $PSScriptRoot 'generate-hpd-v2.py') --upstream $upstreamRoot
if ($LASTEXITCODE -ne 0) { throw 'HPD source generation failed' }
$buildLog = Join-Path $repoRoot '.build\hpd-v2\build-pc-tv-v3.log'
Push-Location $upstreamRoot
try {
    & qmk compile -kb ergohaven/hpd/rev2 -km pc_tv_v3 2>&1 | Tee-Object -FilePath $buildLog
    if ($LASTEXITCODE -ne 0) { throw 'HPD compile failed; see build log' }
} finally { Pop-Location }
& python (Join-Path $PSScriptRoot 'verify-hpd-v2.py')
if ($LASTEXITCODE -ne 0) { throw 'HPD verification failed; do not use the outputs' }
Write-Host 'HPD build and static verification complete. No hardware was flashed.'
