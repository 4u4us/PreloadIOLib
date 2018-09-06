import java.io.*;
import java.util.Random;

public class aJavaApp {
    public static void main(String [] args) {
        // The name of the file to open.
        
        //String fileName = "/proc/textJavaApp.preloadtest";
        
        String fileName = args[0] + "/textJavaApp.preloadtest";
        try {
        	String data = "This is the text content random number=";
			Random rand = new Random();
			int  nAtRandom = rand.nextInt(5000) + 1;
        	data += Integer.toString(nAtRandom);
        	data += "\n";
	        
        	RandomAccessFile file = new RandomAccessFile(fileName,"rw");
	        file.seek(0);
			file.write(file.getFD().toString().getBytes());
	        file.write(data.getBytes());
	        file.close();
		} catch (IOException e) {
			e.printStackTrace();
		} 

    }
}
