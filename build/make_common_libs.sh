#!/bin/bash

LIBS=
PLATFORM=
TARGET_ARCH="x86_64"
TARGET_PLATFORM="osx"
OPENSSL_VERSION="openssl-1.1.1b"

echo "detected OS: $OSTYPE"
if [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="osx"
elif [[ "$OSTYPE" == "linux"* ]]; then
    PLATFORM="linux"
else
    echo "coundn't detect ostype" && exit 1
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

# todo: check rebuild
#rm -rf common_libs

CUR_DIR=$PWD

######### openssl ######### 

if [ ! -d common_libs/$TARGET_PLATFORM/openssl ]; then
    mkdir -p common_libs/$TARGET_PLATFORM/openssl 
fi    

cd common_libs/$TARGET_PLATFORM/openssl 
echo "Unpacking openssl"
tar xfz "../../../../../common_libs/openssl/${OPENSSL_VERSION}.tar.gz"

echo "Building ${OPENSSL_VERSION} for ${TARGET_PLATFORM}, arch ${TARGET_ARCH}"
TARGET="darwin-i386-cc"

DEVELOPER=`xcode-select -print-path`
PL="iPhoneSimulator"
  
export $PL
export CROSS_TOP="${DEVELOPER}/Platforms/${PL}.platform/Developer"
SDK_VERSION="13.2"
export CROSS_SDK="${PL}${SDK_VERSION}.sdk"
export BUILD_TOOLS="${DEVELOPER}"
#export CC="clang"
   

if [[ $TARGET_ARCH == "x86_64" ]]; then
    TARGET="darwin64-x86_64-cc"
fi

cd "${OPENSSL_VERSION}"
./Configure $TARGET
sed -ie "s!^CFLAGS=!CFLAGS=-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK}  -mios-simulator-version-min=${SDK_VERSION} !" "Makefile"

#sed -ie "s!^CFLAG=!CFLAG=-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -miphoneos-version-min=${SDK_VERSION} !" "Makefile"
make || exit 1

lipo "libcrypto.a" -create -output "${LIBS}/libcrypto.a"
lipo "libssl.a" -create -output "${LIBS}/libssl.a"

#cp -f *.a ${LIBS}

cd $CUR_DIR


######### curl ######### 

if [ ! -d common_libs/$TARGET_PLATFORM/curl ]; then
    mkdir -p common_libs/$TARGET_PLATFORM/curl 
fi    
cd common_libs/$TARGET_PLATFORM/curl

cmake ../../../../../common_libs/curl -DUSE_OPENSSL=1 \
-DOPENSSL_DIR="${CUR_DIR}/common_libs/$TARGET_PLATFORM/openssl/${OPENSSL_VERSION}" \
-DCMAKE_TOOLCHAIN_FILE=$CUR_DIR/../toolchains/ios.toolchain.cmake \
    -DPLATFORM=SIMULATOR64

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
    -DPLATFORM=SIMULATOR64
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
    -DPLATFORM=SIMULATOR64

make || exit 1
cp -f *.a $LIBS
cd $CUR_DIR

echo "done"





