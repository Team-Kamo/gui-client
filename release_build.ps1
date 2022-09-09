$path = Split-Path -Parent $MyInvocation.MyCommand.Path
Push-Location $path

cmake -B release -DCMAKE_BUILD_TYPE=Release

if ($?) {
  cmake --build release --config Release
}

if($?) {
  Push-Location release/Release
  windeployqt --dir dist octane.exe
  Pop-Location
}

Pop-Location