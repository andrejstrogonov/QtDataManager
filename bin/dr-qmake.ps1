param([string[]]$args)

$hostWorkDir = (Get-Location).Path
$tempDir = $env:TEMP

docker run --rm -v "${hostWorkDir}:/public/Work" -v "${tempDir}:/tmp" -w /public/Work qt-ubuntu-22.04-qt6 /usr/bin/qmake6 @args