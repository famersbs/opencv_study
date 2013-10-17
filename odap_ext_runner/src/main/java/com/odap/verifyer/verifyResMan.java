package com.odap.verifyer;

import java.awt.Image;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
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
	
}
