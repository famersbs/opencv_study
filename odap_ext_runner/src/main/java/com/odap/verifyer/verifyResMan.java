package com.odap.verifyer;

import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Iterator;
import java.util.List;

import javax.imageio.ImageIO;

import com.odap.util.NADXMLObj;


/**
 * 
 * @author famersbs
 *
 */
public class verifyResMan {

	private BufferedImage 	image = null;
	private Rectangle[] rects = new Rectangle[5];
	private String			image_path = "";
	
	/**
	 * 
	 * @return
	 */
	public BufferedImage getImage(){
		return image;
	}
	
	/**
	 * 
	 * @return
	 */
	public Rectangle[] getRects(){
		return rects;
	}
	
	/**
	 * 
	 * @param imgPath
	 */
	private verifyResMan( String imgPath ){
		image_path = imgPath;
		
		// 리소스 초기화
		for( int i = 0 ; i < 5 ; ++ i ){
			rects[i] = new Rectangle( 0,0,0,0 );
		}
		
		loadImage( imgPath );
	}

	/**
	 * Object Create
	 * @param imgPath
	 * @return
	 */
	public static verifyResMan create( String imgPath ){
		return new verifyResMan( imgPath );
	}
	
	private void loadImage( String imgPath ){
    	try {                
    		image = ImageIO.read(new File(imgPath));

    	} catch (IOException ex) {
    		ex.printStackTrace();
    	}
    	
    	// XML 파일 존재 여부 확인
    	File verify = new File( imgPath + ".xml" );
    	if( !verify.canRead() ) return;
    	
    	// 문제 영역 가져오기
    	try {
			NADXMLObj info = NADXMLObj.createObjectFromFile(imgPath + ".xml" );
			
			List<NADXMLObj> examples = info.getChild("example").getChilds("element");
			
			Iterator<NADXMLObj> i = examples.iterator();
			
			while( i.hasNext() ){
				NADXMLObj cur = i.next();
				
				int num = Integer.parseInt( cur.getAttribute("num") );
				int x = Integer.parseInt( cur.getAttribute("x"));
				int y = Integer.parseInt( cur.getAttribute("y"));
				int w = Integer.parseInt( cur.getAttribute("w"));
				int h = Integer.parseInt( cur.getAttribute("h"));
				
				rects[num - 1] = new Rectangle(x,y,w,h );
				
			}
			
		} catch (FileNotFoundException e) {
			//alert( "" + e );
		} catch (IOException e) {
			//alert( "" + e );
		} catch (Exception e ){
			//alert( "" + e );
		}
    	
    }
	
	/**
	 * 
	 * @return
	 */
	public boolean saveXML(){
		boolean ret = false;
		File fp = new File( image_path + ".xml" );
		
		try {
			OutputStream out = new FileOutputStream( fp );
			
			OdapMetaBuilder builder = new OdapMetaBuilder();
			
			builder.setProperty("file", image_path);
			
			for( int i = 0 ; i< rects.length ; ++ i ){
				builder.addExam( i + 1, rects[i].x, rects[i].y, rects[i].width, rects[i].height);
			}
			
			out.write( builder.toXMLString().getBytes());
			
			out.close();
			
			ret = true;
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return ret;
	}
	
}
