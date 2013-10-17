package com.odap;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

public class odap_ext_runner {

	// 저장 하기
	public static boolean saveUrl(String filename, String urlString)
    {
		File fp = new File( filename.substring(0, filename.lastIndexOf( "/" ) ) );
		fp.mkdirs();
		
        BufferedInputStream in = null;
        FileOutputStream fout = null;
        try
        {
            in = new BufferedInputStream(new URL(urlString).openStream());
            fout = new FileOutputStream(filename);

            byte data[] = new byte[1024];
            int count;
            while ((count = in.read(data, 0, 1024)) != -1)
            {
                fout.write(data, 0, count);
            }
            
            return true;
        }catch( Exception ex ){
        	ex.printStackTrace();
        	return false;
        }
        finally
        {
            if (in != null)
				try {
					in.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
            if (fout != null)
				try {
					fout.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
        }
    }
	
	/**
	 * 
	 * @param proc
	 * @param img_path
	 * @param xml_path
	 * @return
	 */
	public static boolean runProcess( String proc_path, String img_path, String xml_path ){
		String run_cmdline = proc_path + " " + img_path +  " " + xml_path;
		System.out.println( "run : " + run_cmdline );
		try
        {            
            Runtime rt = Runtime.getRuntime();
            Process proc = rt.exec( run_cmdline );
            
            /*----------------------------------------*/
            // 이부분이 없다면 javac 를 실행했을 때 error 가 생긴다.
            InputStream stderr = proc.getErrorStream();
            InputStreamReader isr = new InputStreamReader(stderr);
            BufferedReader br = new BufferedReader(isr);
            String line = null;
            System.out.println("<ERROR>");
            while ( (line = br.readLine()) != null)
                System.out.println(line);
            System.out.println("</ERROR>");
            /*----------------------------------------*/
            
            int exitVal = proc.waitFor();
            System.out.println("Process exitValue: " + exitVal);
            
            return (exitVal == 0 ? true : false );
            
        } catch (Throwable t)
          {
            t.printStackTrace();
          }
		
		return false;
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		String dbUrl = "jdbc:mysql://14.63.184.255/odabnote";
	    String dbClass = "com.mysql.jdbc.Driver";
	    String query = "Select id, image_path from questions order by id DESC limit 100";
	    String username = "odab_ext";
	    String password = "tmakxmgksrhdqntmqrhks";
	    String image_path = "http://cdn.odab.net/img/questions/";
	    
	    String image_temp_path = "res/";
	    String xml_temp_path = "res/";
	    
	    String odap_runner = "/Users/famersbs/git/opencv_study/odap_ext/Debug/odap_ext";
	    //String odap_runner = "./odap_ext";
	    
	    List<String> image_list = new LinkedList<String>();
	    Connection connection = null;
	    
	    // Step 1. 이미지 리스트 추출
	    
	    try {

	        Class.forName(dbClass);
	        
	        connection = DriverManager.getConnection(dbUrl,username, password);
	        Statement statement = connection.createStatement();
	        ResultSet resultSet = statement.executeQuery(query);
	        while (resultSet.next()) {
		        String tableName = resultSet.getString(1);
		        String path = resultSet.getString(2);
		        System.out.println("id : " + tableName +  " " + image_path + path);
		        image_list.add( path );
	        }
	        
	        resultSet.close();
	        statement.close();
	        
	    } catch (ClassNotFoundException e) {
	        e.printStackTrace();
	    } catch (SQLException e) {
	        e.printStackTrace();
	    }finally
        {
	    	if( null != connection ){
	    		try {
					connection.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
        }
	    
        
	    
	    // 임시 URL  등록
	    //image_list.add("130903/130903_G3_KA_01.png");
	    
	    // Step 2. 이미지 다운로드 및 XML 생성
	    Iterator<String> i = image_list.iterator();
	    while( i.hasNext() ){
	    	String cur = i.next();
	    	
	    	// Step 2-1. 이미지 저장
	    	
	    	if( saveUrl( image_temp_path + cur, image_path + cur ) ){
	    		System.out.println( cur + " success" );
	    	}else{
	    		System.out.println( cur + " fail" );
	    		continue;
	    	}
	    	
	    	
	    	// Step 2-2. xml 생성
	    	runProcess( odap_runner, image_temp_path + cur, xml_temp_path + cur + ".xml" );
	    	
	    }
	    //saveUrl( image_temp_path +  )
	    
	    
	}

}
