$path = Split-Path -Parent $MyInvocation.MyCommand.Path
Push-Location $path

cmake -B release -DCMAKE_BUILD_TYPE=Release

if ($?) {
  cmake --build release --config Release
}

Pop-Location