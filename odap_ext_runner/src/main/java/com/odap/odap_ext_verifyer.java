package com.odap;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;

import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.filechooser.FileFilter;

import com.odap.util.NADXMLObj;

public class odap_ext_verifyer {

	/**
	 * 
	 * @author famersbs
	 *
	 */
	public static class Utils {

	    public final static String jpeg = "jpeg";
	    public final static String jpg = "jpg";
	    public final static String gif = "gif";
	    public final static String tiff = "tiff";
	    public final static String tif = "tif";
	    public final static String png = "png";

	    /*
	     * Get the extension of a file.
	     */  
	    public static String getExtension(File f) {
	        String ext = null;
	        String s = f.getName();
	        int i = s.lastIndexOf('.');

	        if (i > 0 &&  i < s.length() - 1) {
	            ext = s.substring(i+1).toLowerCase();
	        }
	        return ext;
	    }
	}
	
	/**
	 * 
	 * @author famersbs
	 *
	 */
	public class ImagePanel extends JPanel{

	    /**
		 * 
		 */
		private static final long serialVersionUID = 1L;
		private BufferedImage image = null;
		private Rectangle[] rects = new Rectangle[5] ;

	    public ImagePanel() {
	    	loadImage( "res/040330/040330_G3_KO_007.gif");
	    }
	    
	    private void alert( String msg ){
	    	image = null;
	    	JOptionPane.showMessageDialog(null,msg,"Image Load Fail",JOptionPane.WARNING_MESSAGE);
	    }
	    
	    public void loadImage( String imgPath ){
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
				alert( "" + e );
			} catch (IOException e) {
				alert( "" + e );
			} catch (Exception e ){
				alert( "" + e );
			}
	    	
	    	// 다시 그리기
    		repaint();
	    }

	    @Override
	    protected void paintComponent(Graphics g) {
	        super.paintComponent(g);
	        if( null != image ){
	        	g.drawImage(image, 0, 0, null); // see javadoc for more info on the parameters 
	        	
	        	g.setColor( new Color( 0x00FF00 ) );
	        	for( int i = 0 ; i < rects.length ; ++ i ){
	        		g.drawRect(rects[i].x,rects[i].y,rects[i].width,rects[i].height );
	        	}
	        }
	    }

	}
	
	public void go() {
		final JFrame frame = new JFrame("odab ext verifyer");// 프레임 생성
		
		JPanel panel = new JPanel( new BorderLayout() );// 패널 생성
		
			// Image 
		final ImagePanel img_panel = new ImagePanel();
		final JFileChooser fc = new JFileChooser();
		fc.setCurrentDirectory(new File("./") );
		fc.addChoosableFileFilter(new FileFilter(){

			public boolean accept(File f) {
				if (f.isDirectory()) {
			        return true;
			    }

			    String extension = Utils.getExtension(f);
			    if (extension != null) {
			        if (extension.equals(Utils.tiff) ||
			            extension.equals(Utils.tif) ||
			            extension.equals(Utils.gif) ||
			            extension.equals(Utils.jpeg) ||
			            extension.equals(Utils.jpg) ||
			            extension.equals(Utils.png)) {
			                return true;
			        } else {
			            return false;
			        }
			    }

			    return false;
			}

			@Override
			public String getDescription() {
				return null;
			}
			
		});
		
		
			// tool bar 
		JToolBar toolBar = new JToolBar("Still draggable");
		JButton button = new JButton("open file");// 버튼 생성
		
		button.addActionListener( new ActionListener(){

			public void actionPerformed(ActionEvent event) {
				
				int returnVal = fc.showOpenDialog( frame );

		        if (returnVal == JFileChooser.APPROVE_OPTION) {
		            File file = fc.getSelectedFile();
		            img_panel.loadImage(file.getAbsolutePath());
		        }
			}
			
		});
		
		toolBar.add(button);

		
			// panel add
		panel.add( toolBar , BorderLayout.PAGE_START );
		panel.add( img_panel, BorderLayout.CENTER );
		
		frame.getContentPane().add(panel);
		frame.setSize(300,1000);
		frame.setVisible(true);
		frame.setResizable( true );
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		new odap_ext_verifyer().go();
	}

}
