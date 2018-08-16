all:
	g++ -g -std=c++11 -fPIC -shared -o preloadFunctions.so preloadWrite.c -ldl -lpthread
	g++ -g aLinuxApp.cpp
	javac aJavaApp.java
	jar cfm aJavaApp.jar manifest.txt aJavaApp.class
	

clean:
	rm *.o *.so a.out core *.preloadtest *.class *.jar
