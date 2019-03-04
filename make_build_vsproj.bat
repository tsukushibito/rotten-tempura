cd /d %~dp0
mkdir build > NUL 2>&1
pushd build
cmake ../source -G"Visual Studio 15 2017 Win64"
popd