cd /d %~dp0
mkdir build > NUL 2>&1
pushd build
cmake ../source -G"Visual Studio 16 2019" -A x64 -DBOOST_ROOT:PATH=%BOOST_ROOT% -DBOOST_INCLUDEDIR:PATH=%BOOST_INCLUDEDIR% -DBOOST_LIBRARYDIR:PATH=%BOOST_LIBRARYDIR%
popd