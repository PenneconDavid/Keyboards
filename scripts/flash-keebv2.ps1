<#
.SYNOPSIS
    Build (and optionally flash) the keebv2 keymap for the wired Corne (crkbd rev1, Pro Micro).

.DESCRIPTION
    Copies keyboards/crkbd/keymaps/keebv2 from this repo into the local qmk_firmware tree,
    compiles it, then flashes the same .hex to each half in turn. The ATmega32U4 Pro Micros run
    the Caterina bootloader: double-tap reset (or short RST to GND) and a new COM port appears
    for about eight seconds. By default `qmk flash` watches for that new port itself, so no COM
    numbers are needed; pass -LeftPort / -RightPort only if auto-detection fails on your machine.

    Run from an ordinary PowerShell. If `qmk` is the QMK MSYS one, the script sets the
    environment it needs (MSYSTEM, SHELL, toolchain PATH).

.EXAMPLE
    .\scripts\flash-keebv2.ps1 -BuildOnly        # compile only, hex ends up in $QmkPath
    .\scripts\flash-keebv2.ps1                   # compile, then flash left, then right
    .\scripts\flash-keebv2.ps1 -LeftPort COM5 -RightPort COM6
#>
[CmdletBinding()]
param(
    [string]$QmkPath = "$env:USERPROFILE\qmk_firmware",
    [string]$Keymap = "keebv2",
    [string]$LeftPort,
    [string]$RightPort,
    [switch]$BuildOnly
)

$ErrorActionPreference = 'Stop'

function Invoke-Qmk {
    param([Parameter(Mandatory)][string[]]$Arguments)
    Write-Host "`n>>> qmk $($Arguments -join ' ')" -ForegroundColor Cyan
    & qmk @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "qmk $($Arguments -join ' ') failed with exit code $LASTEXITCODE"
    }
}

function Invoke-Flash {
    param([Parameter(Mandatory)][string]$Half, [string]$Port)
    Write-Host "`n=== Flashing $Half half ===" -ForegroundColor Green
    Write-Host "Plug in the $Half half alone, then press Enter and put it into the bootloader (double-tap reset)..." -NoNewline
    [void][System.Console]::ReadLine()
    $flashArgs = @('flash', '-kb', 'crkbd', '-km', $Keymap, '-bl', 'avrdude')
    if ($Port) { $flashArgs += @('-P', $Port) }
    Invoke-Qmk $flashArgs
}

if (-not (Get-Command qmk -ErrorAction SilentlyContinue)) {
    throw "The 'qmk' CLI is not in PATH. Install QMK MSYS or run 'pip install qmk' first."
}

# The QMK MSYS build of the CLI refuses to run outside its own shell unless these are set.
$qmkMsys = 'C:\Program Files\QMK_MSYS'
if (Test-Path $qmkMsys) {
    if (-not $env:MSYSTEM) { $env:MSYSTEM = 'MINGW64' }
    if (-not $env:SHELL) { $env:SHELL = "$qmkMsys\usr\bin\bash.exe" }
    $env:PATH = "$qmkMsys\mingw64\bin;$qmkMsys\usr\bin;$env:PATH"
}

if (-not (Test-Path $QmkPath)) {
    throw "QMK firmware path '$QmkPath' not found. Pass -QmkPath if you keep it elsewhere."
}

$repoRoot = Split-Path $PSScriptRoot -Parent
$keymapSrc = Join-Path $repoRoot "keyboards\crkbd\keymaps\$Keymap"
if (-not (Test-Path $keymapSrc)) {
    throw "Keymap '$keymapSrc' not found."
}
$keymapDst = Join-Path $QmkPath "keyboards\crkbd\keymaps\$Keymap"
New-Item -ItemType Directory -Force -Path $keymapDst | Out-Null
Copy-Item -Force (Join-Path $keymapSrc '*') $keymapDst
Write-Host "Copied $Keymap into $keymapDst" -ForegroundColor Cyan

Push-Location $QmkPath
try {
    Invoke-Qmk @('compile', '-kb', 'crkbd', '-km', $Keymap)
    $hex = Join-Path $QmkPath "crkbd_rev1_$Keymap.hex"
    Write-Host "Built $hex" -ForegroundColor Cyan

    if ($BuildOnly) {
        Write-Host "Build completed. Skipping flashing because -BuildOnly was provided." -ForegroundColor Yellow
        return
    }

    # One hex serves both halves (MASTER_LEFT + SPLIT_USB_DETECT decide the role at runtime).
    Invoke-Flash -Half 'left' -Port $LeftPort
    Invoke-Flash -Half 'right' -Port $RightPort

    Write-Host "`nAll done. Reconnect both halves over TRRS, plug USB into the left, and check the OLEDs." -ForegroundColor Green
    Write-Host "Then export the live layout from https://usevia.app into layouts/via/crkbd.layout.<yyyy-MM-dd>.json." -ForegroundColor Green
}
finally {
    Pop-Location
}
