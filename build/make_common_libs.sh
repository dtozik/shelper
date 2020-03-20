#!/bin/bash

LIBS=
OS=
TARGET_PLATFORM=
OPENSSL_VERSION="openssl-1.1.1b"
SDK_VERSION="13.2"
DEVELOPER=`xcode-select -print-path`
SDK_VERSION=$(xcrun --sdk iphoneos --show-sdk-version)

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
      shift 1
      ;;
    --osx)
      TARGET_PLATFORM="osx"
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
    mkdir -p projects
fi
cd projects

if [ ! -d libs ]; then
    mkdir -p libs
fi
LIBS="$PWD/libs"

echo $LIBS


rm -rf common_libs/$TARGET_PLATFORM
rm -rf $LIBS/*.*

CUR_DIR=$PWD

######### openssl ######### 

build_ios_ssl_impl()
{
    ARCH=$1

    if [[ "${ARCH}" == "i386" || "${ARCH}" == "x86_64" ]]; then
        PLATFORM="iPhoneSimulator"
    else
        PLATFORM="iPhoneOS"
        sed -ie "s!static volatile sig_atomic_t intr_signal;!static volatile intr_signal;!" "crypto/ui/ui_openssl.c"
    fi
  
    export $PLATFORM
    export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
    export CROSS_SDK="${PLATFORM}${SDK_VERSION}.sdk"
    export BUILD_TOOLS="${DEVELOPER}"
    export CC="${BUILD_TOOLS}/usr/bin/gcc -arch ${ARCH}"
   
    echo "Building ${OPENSSL_VERSION} for ${PLATFORM} ${SDK_VERSION} ${ARCH}"

    if [[ "${ARCH}" == "x86_64" ]]; then
        ./Configure darwin64-x86_64-cc
        sed -ie "s!^CFLAGS=!CFLAGS=-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK}  -mios-simulator-version-min=${SDK_VERSION} !" "Makefile"
    else
        ./Configure iphoneos-cross
        sed -ie "s!^CFLAG=!CFLAG=-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -miphoneos-version-min=${SDK_VERSION} !" "Makefile"
    fi
    make
}

if [ ! -d common_libs/$TARGET_PLATFORM/openssl ]; then
    mkdir -p common_libs/$TARGET_PLATFORM/openssl 
fi    
cd common_libs/$TARGET_PLATFORM/openssl

build_ios_ssl()
{
    ARCH=$1
    rm -rf $OPENSSL_VERSION 
    echo "Unpacking openssl"
    tar xfz "../../../../../common_libs/openssl/${OPENSSL_VERSION}.tar.gz"
    cd $OPENSSL_VERSION
    build_ios_ssl_impl $ARCH
    cp -f "libcrypto.a" "${LIBS}/libcrypto-${ARCH}.a"
    cp -f "libssl.a" "${LIBS}/libssl-${ARCH}.a"
    cd ..
}

#build_ios_ssl "armv7"
#build_ios_ssl "arm64"

build_ios_ssl "x86_64"

lipo \
     "${LIBS}/libcrypto-x86_64.a" \
    -create -output "${LIBS}/libcrypto.a"

lipo \
     "${LIBS}/libssl-x86_64.a" \
    -create -output "${LIBS}/libssl.a"


cd $CUR_DIR

######### curl ######### 

if [ ! -d common_libs/$TARGET_PLATFORM/curl ]; then
    mkdir -p common_libs/$TARGET_PLATFORM/curl 
fi    
cd common_libs/$TARGET_PLATFORM/curl

cmake ../../../../../common_libs/curl -DUSE_OPENSSL=1 \
-DOPENSSL_DIR="${CUR_DIR}/common_libs/$TARGET_PLATFORM/openssl/${OPENSSL_VERSION}" \
-DCMAKE_TOOLCHAIN_FILE=$CUR_DIR/../toolchains/ios.toolchain.cmake \
    -DPLATFORM=SIMULATOR64 -DSDK_VERSION=$SDK_VERSION

make || exit 1
cp -f *.a $LIBS
cd $CUR_DIR

######### curlpp-0.8.1 #########
if [ ! -d common_libs/$TARGET_PLATFORM/curlpp-0.8.1 ]; then
    mkdir -p common_libs/$TARGET_PLATFORM/curlpp-0.8.1 
fi    
cd common_libs/$TARGET_PLATFORM/curlpp-0.8.1

CURL_INCLUDE_PATH="${CUR_DIR}/../../common_libs/curl/curl-7.64.1/include"

cmake ../../../../../common_libs/curlpp-0.8.1 -DCURL_INCLUDE_DIRS="${CURL_INCLUDE_PATH}" \
-DCMAKE_TOOLCHAIN_FILE=$CUR_DIR/../toolchains/ios.toolchain.cmake \
    -DPLATFORM=SIMULATOR64 -DSDK_VERSION=$SDK_VERSION
make || exit 1
cp -f *.a $LIBS
cd $CUR_DIR

########## zlib-1.2.5 #########
if [ ! -d common_libs/$TARGET_PLATFORM/zlib ]; then
    mkdir -p common_libs/$TARGET_PLATFORM/zlib 
fi    
cd common_libs/$TARGET_PLATFORM/zlib

cmake ../../../../../common_libs/zlib \
-DCMAKE_TOOLCHAIN_FILE=$CUR_DIR/../toolchains/ios.toolchain.cmake \
    -DPLATFORM=SIMULATOR64 -DSDK_VERSION=$SDK_VERSION

make || exit 1
cp -f *.a $LIBS
cd $CUR_DIR

echo "done"





