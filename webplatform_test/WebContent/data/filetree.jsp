<%@page import="test.sencha_test_listner"%>
<%@page import="java.io.File"%>
<%@page import="java.util.Iterator"%><%
	/*
	Iterator<String> param_i = request.getParameterMap().keySet().iterator();

	
	while( param_i.hasNext() ){
		String key = param_i.next();
		System.out.println( key + " " + request.getParameter( key ) );
	}
	
	System.out.println("---------------------------------------------");
	*/
	
	
	
	// Node Null 처리
	if( null == request.getParameter("node") ||
		request.getParameter("node").equals("NULL") ){
		
		// 일단 파일 Depth 는 1래밸로 한다.
		// 파일 경로를 훑어 봅시다.		
		String img_path = sencha_test_listner.imagestore_root_path;
		
		File root = new File( img_path );
		
		File[] list = root.listFiles();
%>{
   "divisions":[
	<% 
		for( int i = 0; i < list.length ; ++ i ){
			if( !list[i].isDirectory() ) continue;
	%>
      {
         "name":"<%=list[i].getName() %>",
         "id":"<%=list[i].getName() %>",  
      },
	<% 
		}
 	%>    	
   ]
}
<% } %>