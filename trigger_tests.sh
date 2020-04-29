
set -e

cmake --no-warn-unused-cli -H. -B./build -G "Unix Makefiles"

cmake --build build --config Debug --target all
./build/ElectionTester