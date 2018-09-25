// Multithreaded java app
// /home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/bin/javac aJavaMultithreadedApp.java
// /home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/bin/jar cfm aJavaMultithreadedApp.jar manifestM.txt aJavaMultithreadedApp.class
// /home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/bin/java -cp .:/home/${USER}/zulu8.30.0.1-jdk8.0.172-linux_i686/lib -jar aJavaMultithreadedApp.jar /home/${USER}	
import java.io.*;
import java.util.Random;

class aJavaMultithreadedApp extends Thread{  

  public static int DO_WRITE = 100; 
  public static int DO_READ = 200;

  protected String m_filename = "";
  protected int m_actionId = 0;

  aJavaMultithreadedApp(String a_filename, int a_actionId){
	  m_filename = a_filename;
	  m_actionId = a_actionId;
  } 
	
	
  public void run(){  
    System.out.println("File thread " + m_filename + " started!"); 
	if (DO_WRITE==m_actionId){
		for(int i=0;i<1000;i++){
			String fileName = m_filename;
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
	else if (DO_READ==m_actionId){
		for(int i=0;i<5;i++){
			String fileName = m_filename;
			try {
				RandomAccessFile file = new RandomAccessFile(fileName,"rw");
				file.seek(0);
				byte[] rdbuf = new byte[80];
				file.read(rdbuf);
				file.close();
				char c;
				 for(byte b:rdbuf) {
					c = (char)b;
					System.out.print(c);
				 }
				System.out.println("");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}		
		
	}
	  
  }   
	
  public static void main(String args[]){  
     aJavaMultithreadedApp file_1 = new aJavaMultithreadedApp(args[0] + "/File_1.txt",DO_WRITE);   
     file_1.start();  
     aJavaMultithreadedApp file_2 = new aJavaMultithreadedApp(args[0] + "/File_1.txt",DO_READ);   
     file_2.start();
  }  
	
}

