#!/bin/sh

make clean

make

echo "___________________________"
echo "JAVA"
echo "___________________________"

LD_PRELOAD=preloadFunctions.so java -jar aJavaApp.jar

echo "___________________________"
echo "JAVA Result"
echo "___________________________"
more textJavaApp.preloadtest 

echo "___________________________"
echo "___________________________"
echo "LINUX"
echo "___________________________"

LD_PRELOAD=preloadFunctions.so ./a.out

echo "___________________________"
echo "LINUX Result"
echo "___________________________"
more textLinuxApp.preloadtest 
