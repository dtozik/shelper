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
    mkdir -p projects
fi
cd projects

if [ ! -d libs ]; then
    mkdir -p libs
fi
LIBS="$PWD/libs"

echo $LIBS
rm -rf common_libs

CUR_DIR=$PWD

######### openssl ######### 

# if [ ! -d common_libs/$PLATFORM_NAME/openssl ]; then
#     mkdir -p common_libs/$PLATFORM_NAME/openssl 
# fi    
# #cd common_libs/$PLATFORM_NAME/openssl
# cd ../../common_libs/openssl
# ./openssl-build.sh

# #cmake ../../../../../common_libs/curl -DUSE_OPENSSL=1
# #make || exit 1
# #cp -rf *.a $LIBS
# cd $CUR_DIR

######### curl ######### 

if [ ! -d common_libs/$PLATFORM_NAME/curl ]; then
    mkdir -p common_libs/$PLATFORM_NAME/curl 
fi    
cd common_libs/$PLATFORM_NAME/curl

cmake ../../../../../common_libs/curl #-DUSE_OPENSSL=1
make || exit 1
cp -rf *.a $LIBS
cd $CUR_DIR

######### curlpp-0.8.1 #########
if [ ! -d common_libs/$PLATFORM_NAME/curlpp-0.8.1 ]; then
    mkdir -p common_libs/$PLATFORM_NAME/curlpp-0.8.1 
fi    
cd common_libs/$PLATFORM_NAME/curlpp-0.8.1

cmake ../../../../../common_libs/curlpp-0.8.1
make || exit 1
cp -rf *.a $LIBS
cd $CUR_DIR

########## zlib-1.2.5 #########
if [ ! -d common_libs/$PLATFORM_NAME/zlib ]; then
    mkdir -p common_libs/$PLATFORM_NAME/zlib 
fi    
cd common_libs/$PLATFORM_NAME/zlib

cmake ../../../../../common_libs/zlib
make || exit 1
cp -rf *.a $LIBS
cd $CUR_DIR

echo "done"





