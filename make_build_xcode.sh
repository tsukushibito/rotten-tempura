#!/bin/sh
cd `dirname $0`
mkdir -p build
pushd build
cmake ../source -GXcode
popd