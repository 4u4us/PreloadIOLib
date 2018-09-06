all:
	g++ -g -std=c++11 -fPIC -shared -o preloadFunctions.so preloadWrite.c -ldl -lpthread
	g++ -g aLinuxApp.cpp
	/home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/bin/javac aJavaApp.java
	/home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/bin/jar cfm aJavaApp.jar manifest.txt aJavaApp.class
	

clean:
	rm *.o *.so a.out core /home/${USER}/*.preloadtest *.preloadtest *.class *.jar modified_file_*
