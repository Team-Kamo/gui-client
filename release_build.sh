#!/bin/sh
cd `dirname $0`
cmake -B release -DCMAKE_BUILD_TYPE=Release
cmake --build release --config Release
