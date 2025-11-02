param(
    [string]$QmkPath = "$env:USERPROFILE\qmk_firmware",
    [string]$Keymap = "keebv2",
    [string]$LeftPort = "COM5",
    [string]$RightPort = "COM6",
    [switch]$BuildOnly
)

function Invoke-QmkCommand {
    param([string]$Args)
    Write-Host "\n>>> qmk $Args" -ForegroundColor Cyan
    & qmk $Args
    if ($LASTEXITCODE -ne 0) {
        throw "qmk $Args failed with exit code $LASTEXITCODE"
    }
}

if (-not (Get-Command qmk -ErrorAction SilentlyContinue)) {
    throw "The 'qmk' CLI is not in PATH. Launch QMK MSYS or run 'pip install qmk' first."
}

if (-not (Test-Path $QmkPath)) {
    throw "QMK firmware path '$QmkPath' not found. Pass -QmkPath to this script if you keep it elsewhere."
}

Push-Location $QmkPath
try {
    Invoke-QmkCommand "compile -kb crkbd -km $Keymap"

    if ($BuildOnly) {
        Write-Host "Build completed. Skipping flashing because -BuildOnly was provided." -ForegroundColor Yellow
        return
    }

    Write-Host "\n=== Flashing left (master) half on $LeftPort ===" -ForegroundColor Green
    Write-Host "Put the left half into bootloader (hold reset) then press Enter..." -NoNewline
    [void][System.Console]::ReadLine()
    Invoke-QmkCommand "flash -kb crkbd -km $Keymap -bl avrdude -c avr109 -p ATmega32U4 -P $LeftPort"

    Write-Host "\n=== Flashing right half on $RightPort ===" -ForegroundColor Green
    Write-Host "Swap USB to the right half, enter bootloader, then press Enter..." -NoNewline
    [void][System.Console]::ReadLine()
    Invoke-QmkCommand "flash -kb crkbd -km $Keymap -bl avrdude -c avr109 -p ATmega32U4 -P $RightPort"

    Write-Host "\nAll done! Plug both halves back in to verify split sync." -ForegroundColor Green
}
finally {
    Pop-Location
}

                                                        