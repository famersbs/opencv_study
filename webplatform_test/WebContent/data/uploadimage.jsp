<%@page import="test.fileproc"%>
<%@page import="java.io.File"%>
<%@page import="test.sencha_test_listner"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
 <%@ page import="java.util.*"%>
 <%@ page import="com.oreilly.servlet.MultipartRequest"%>
 <%@ page import="com.oreilly.servlet.multipart.DefaultFileRenamePolicy"%><%
 	String message = "Success";
  	String fileName = "";
  	String title = "";
  	String fullPath = sencha_test_listner.imagestore_root_path;
  	try{
  		
 	    MultipartRequest multi = new MultipartRequest(request, sencha_test_listner.imagestore_root_path, 30*1024*1024, "utf-8", new DefaultFileRenamePolicy() );

 	    fileName=multi.getFilesystemName("photo");
 	    title = multi.getParameter("name");
 	    
 	   
 	    // 디렉터리 생성
 	    
 	   	File fp = new File( fullPath + "/" + title );
 	   	fp.mkdir();
 	   	
 	   	// 파일 이동
 	   	fileproc.fileMove(fullPath + "/" + fileName, fullPath + "/" + title + "/" + "origin.png" );
 	   	
 	   	// 프로세스 실행
 	    fileproc.runImgProcess( fullPath + "/" + title );
 	   
 	    
 	 } catch(Exception e) {
 	     message = "fail to upload file";
 	    e.printStackTrace();
 	 } 
 	
 %>
