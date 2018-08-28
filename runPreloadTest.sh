#!/bin/sh

make clean

make

echo "___________________________"
echo "JAVA"
echo "___________________________"

LD_PRELOAD=preloadFunctions.so /home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/bin/java -cp .:/home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/lib -jar aJavaApp.jar
                                                                                                         
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
