<%@page import="test.sencha_test_listner"%>
<%@page import="java.io.FileInputStream"%>
<%@page import="java.io.BufferedOutputStream"%>
<%@page import="java.io.BufferedInputStream"%>
<%@page import="java.io.File"%>
<%@ page contentType="application/octet-stream;charset=UTF-8" %><%
	
	response.setHeader("Content-Type","image/png"); //다운로드할 파일 형식 결정.
	
	String pathname = request.getParameter("name"); // 요청페이지 에서 넘어온 파일 경로
	String typename	= request.getParameter("type"); // 요청페이지 에서 넘어온 파일 경로
	
	String dFileName= typename + ".png";

	String path = sencha_test_listner.imagestore_root_path + "/" + pathname + "/";
	File file = new File(path+dFileName);
	byte b[] = new byte[(int)file.length()];
	
	System.out.println( "size : " + b.length); 
	System.out.println( "filename : " + dFileName); 
	System.out.println( "path : " + path); 
	
	response.setHeader("Content-Disposition", "attachment;filename=" + dFileName + ";");
	if (file.isFile())
	{
		BufferedInputStream fin = new BufferedInputStream(new FileInputStream(file));
		BufferedOutputStream outs = new BufferedOutputStream(response.getOutputStream());
		int read = 0;
		while ((read = fin.read(b)) != -1){
			outs.write(b,0,read);
		}
		outs.close();
		fin.close();
	}
%>