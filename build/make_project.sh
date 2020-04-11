#!/bin/bash

LIBS=
OS=
TARGET_PLATFORM=
SDK_VERSION=$(xcrun --sdk iphoneos --show-sdk-version)
ADDITIONAL_PARAMS=
echo "detected OS: $OSTYPE"
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS="osx"
elif [[ "$OSTYPE" == "linux"* ]]; then
    OS="linux"
else
    echo "coundn't detect ostype" && exit 1
fi


while [ $# -ne 0 ]; do
  case $1 in
    --ios)
      TARGET_PLATFORM="ios"
      ADDITIONAL_PARAMS="$ADDITIONAL_PARAMS -DTARGET_IOS=1 -DCMAKE_TOOLCHAIN_FILE=../../toolchains/ios.toolchain.cmake \
	-DSDK_VERSION=$SDK_VERSION -DDEPLOYMENT_TARGET=$SDK_VERSION"
      shift 1
      ;;
    --osx)
      TARGET_PLATFORM="osx"
      ADDITIONAL_PARAMS="$ADDITIONAL_PARAMS -DTARGET_OSX=1"
      shift 1
      ;;
    *)
      shift 1
      ;;
  esac
done

if [ -z $TARGET_PLATFORM ]; then
    echo "target isn't specified"
    exit 1
fi

if [ ! -d projects ]; then
	mkdir projects
fi

cd projects

if [ ! -d $TARGET_PLATFORM ]; then
    mkdir $TARGET_PLATFORM 
fi

cd $TARGET_PLATFORM

if [[ $OS == "osx" ]]; then

	cmake ../../../ -G "Xcode" -DPLATFORM=OS $ADDITIONAL_PARAMS

    #cmake ../../../ -G "Xcode" -DTARGET_IOS=1 -DTEST_SUB="shawshank.srt"
fi




