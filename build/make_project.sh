#!/bin/bash

BUILD_OSX=0
BUILD_LINUX=0
LIBS=

PLATFORM_NAME=

echo "detected OS: $OSTYPE"
if [[ "$OSTYPE" == "darwin"* ]]; then
    BUILD_OSX=1
    PLATFORM_NAME="osx"
elif [[ "$OSTYPE" == "linux"* ]]; then
    BUILD_LINUX=1
    PLATFORM_NAME="linux"
else
    echo "coundn't detect ostype" && exit 1
fi


if [ ! -d projects ]; then
	mkdir projects
fi

cd projects


if [ ! -d $PLATFORM_NAME ]; then
    mkdir $PLATFORM_NAME 
fi

cd $PLATFORM_NAME

if [[ $BUILD_OSX == 1 ]]; then
    cmake ../../../ -G "Xcode"
fi




