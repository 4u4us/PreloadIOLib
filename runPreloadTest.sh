#!/bin/sh

echo !!! JVM + JDK expected in location:
echo /home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686
echo !!!

sleep 3s

make clean

make

echo "___________________________"
echo "JAVA"
echo "___________________________"

LD_PRELOAD=preloadFunctions.so /home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/bin/java -cp .:/home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/lib -jar aJavaApp.jar /home/${USER}

# Test on read-only containing folder, pass as param to aJavaApp.jar:
#LD_PRELOAD=preloadFunctions.so /home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/bin/java -cp .:/home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/lib -jar aJavaApp.jar /proc
                                                                                                         
echo "___________________________"
echo "JAVA Result"
echo "___________________________"
more /home/${USER}/textJavaApp.preloadtest 

echo "___________________________"
echo "___________________________"
echo "LINUX"
echo "___________________________"

LD_PRELOAD=preloadFunctions.so ./a.out

echo "___________________________"
echo "LINUX Result"
echo "___________________________"
more textLinuxApp.preloadtest 
