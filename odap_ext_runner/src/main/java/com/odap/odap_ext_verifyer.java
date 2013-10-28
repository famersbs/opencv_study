package com.odap;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.filechooser.FileFilter;

import com.odap.verifyer.ImagePanel;

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
		JButton open_btn = new JButton("open file");// 파일 오픈
		JButton save_btn = new JButton("save file");// 파일 저장
		JButton[] number_btns = new JButton[6];		// 에디트 버튼
		final JLabel	status_lbl = new JLabel(" no edit");	// 에디트 상황 라벨
		
		open_btn.addActionListener( new ActionListener(){

			public void actionPerformed(ActionEvent event) {
				
				int returnVal = fc.showOpenDialog( frame );

		        if (returnVal == JFileChooser.APPROVE_OPTION) {
		            File file = fc.getSelectedFile();
		            img_panel.loadImage(file.getAbsolutePath());
		        }
			}
			
		});
		
		save_btn.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent event) {
				img_panel.saveXML();
			}
		});
		

		toolBar.add(open_btn);
		toolBar.add(save_btn);
		
		
		
		for( int i = 0 ; i < number_btns.length; ++ i ){
			String label = "" + ( i );
			if( i == 0 ) label = "no edit";
				
			number_btns[i] = new JButton( label );
			
			number_btns[i].addActionListener(new ActionListener(){
				public void actionPerformed(ActionEvent event) {
					String label = ((JButton) event.getSource()).getText();
					int cur_idx = 0;
					
					if( !"no edit".equals(label) )
						cur_idx = Integer.parseInt( label );
					
					img_panel.setEditNumber( cur_idx );
					
					status_lbl.setText( " " + label );
				}
			});
			
			toolBar.add(number_btns[i]);
			
		}
		
		toolBar.add(status_lbl);
		
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
