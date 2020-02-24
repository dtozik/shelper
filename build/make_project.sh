#!/bin/bash

LIBS=
PLATFORM=
TARGET_PLATFORM="osx"

echo "detected OS: $OSTYPE"
if [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="osx"
elif [[ "$OSTYPE" == "linux"* ]]; then
    PLATFORM="linux"
else
    echo "coundn't detect ostype" && exit 1
fi


if [ ! -d projects ]; then
	mkdir projects
fi

cd projects

if [ ! -d $TARGET_PLATFORM ]; then
    mkdir $TARGET_PLATFORM 
fi

cd $TARGET_PLATFORM

if [[ $PLATFORM == "osx" ]]; then
    cmake ../../../ -G "Xcode" -DTARGET_COCOA=1 -DTEST_SUB="shawshank.srt"
fi




