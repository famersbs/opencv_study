package test;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.List;


public class fileproc {

	 //파일을 존재여부를 확인하는 메소드
	 public static Boolean fileIsLive(String isLivefile) {
	  File f1 = new File(isLivefile);
	  
	  if(f1.exists())
	  {
	   return true;
	  }else
	  {
	   return false;
	  }
	 }
	 
	 //파일을 생성하는 메소드
	 public static void fileMake(String makeFileName) {
	  File f1 = new File(makeFileName);
	  try {
	   f1.createNewFile();
	  } catch (IOException e) {
	   // TODO Auto-generated catch block
	   e.printStackTrace();
	  }
	 }
	 
	 //파일을 삭제하는 메소드
	 public static void fileDelete(String deleteFileName) {
	  File I = new File(deleteFileName);
	  I.delete();
	 }
	 
	 //파일을 복사하는 메소드
	 public static void fileCopy(String inFileName, String outFileName) {
	  try {
	   FileInputStream fis = new FileInputStream(inFileName);
	   FileOutputStream fos = new FileOutputStream(outFileName);
	   
	   int data = 0;
	   while((data=fis.read())!=-1) {
	    fos.write(data);
	   }
	   fis.close();
	   fos.close();
	   
	  } catch (IOException e) {
	   // TODO Auto-generated catch block
	   e.printStackTrace();
	  }
	 }
	 
	 //파일을 이동하는 메소드
	 public static void fileMove(String inFileName, String outFileName) {
	  try {
	   FileInputStream fis = new FileInputStream(inFileName);
	   FileOutputStream fos = new FileOutputStream(outFileName);
	   
	   int data = 0;
	   while((data=fis.read())!=-1) {
	    fos.write(data);
	   }
	   fis.close();
	   fos.close();
	   
	   //복사한뒤 원본파일을 삭제함
	   fileDelete(inFileName);
	   
	  } catch (IOException e) {
	   // TODO Auto-generated catch block
	   e.printStackTrace();
	  }
	 }
	 
	 //디렉토리의 파일 리스트를 읽는 메소드
	 public static List<File> getDirFileList(String dirPath)
	 {
	  // 디렉토리 파일 리스트
	  List<File> dirFileList = null;
	  
	  // 파일 목록을 요청한 디렉토리를 가지고 파일 객체를 생성함
	  File dir = new File(dirPath);
	  
	  // 디렉토리가 존재한다면
	  if (dir.exists())
	  {
	   // 파일 목록을 구함
	   File[] files = dir.listFiles();
	   
	   // 파일 배열을 파일 리스트로 변화함 
	   dirFileList = Arrays.asList(files);
	  }
	  
	  return dirFileList;
	 }
	 
	 public static void runImgProcess( String target_path ){
		 String s = "";
		 try {
		    /*  자바 1.4 이하에서는 이렇게
		    Runtime oRuntime = Runtime.getRuntime();
		    Process oProcess = oRuntime.exec("cmd /c dir /?");
		    */
		    // 자바 1.5 이상에서는 이렇게 1줄로
		    
		    ProcessBuilder builder = new ProcessBuilder( sencha_test_listner.process, target_path );
		    builder.directory(new File( "C:\\Users\\Administrator\\Desktop\\OpenCV_스터디\\project\\opencv_proc\\Debug" ) );
		    Process oProcess = builder.start();  

		    // 외부 프로그램 출력 읽기
		    BufferedReader stdOut   = new BufferedReader(new InputStreamReader(oProcess.getInputStream()));
		    BufferedReader stdError = new BufferedReader(new InputStreamReader(oProcess.getErrorStream()));

		    // "표준 출력"과 "표준 에러 출력"을 출력
		    while ((s = stdOut.readLine()) != null) System.out.println(s);
		    while ((s = stdError.readLine()) != null)System.out.println(s);

		  } catch (IOException e) { // 에러 처리
		      System.err.println("에러! 외부 명령 실행에 실패했습니다.\n" + e.getMessage());
		      System.exit(-1);
		  }
	 }
	 
}

