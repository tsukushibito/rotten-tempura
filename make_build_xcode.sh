#!/bin/sh
cd `dirname $0`
pushd build
cmake ../source -GXcode
popd