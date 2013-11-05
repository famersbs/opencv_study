package com.odap;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.text.NumberFormat;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFormattedTextField;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.JTree;
import javax.swing.SpringLayout;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.filechooser.FileFilter;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import javax.swing.text.NumberFormatter;

import layout.SpringUtilities;

import com.javaswingcomponents.accordion.JSCAccordion;
import com.javaswingcomponents.accordion.TabOrientation;
import com.javaswingcomponents.accordion.listener.AccordionEvent;
import com.javaswingcomponents.accordion.listener.AccordionListener;
import com.odap.util.NADXMLObj;
import com.odap.verifyer.ImagePanel;
import com.odap.verifyer.ImagePanelNumberChooser;
import com.odap.verifyer.ImagePanelNumberChooser_FixedDraw;
import com.odap.verifyer.ImagePanelNumberChooser_FreeDraw;
import com.odap.verifyer.ImagePanelNumberChooser_NoEdit;

public class odap_ext_verifyer {
	
	public String respath = "./";

	public odap_ext_verifyer( String config_path ) throws FileNotFoundException, IOException{
		
		// config load
		NADXMLObj config = NADXMLObj.createObjectFromFile( config_path );
		respath = config.getChild("respath").getContent();
		
	}
	
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
	 * Tool Bar 를 생성 해 주는 함수
	 * @param toolBar
	 * @param img_panel 
	 * @param frame 
	 */
	private void addToToolbar(JToolBar toolBar, final JFrame frame, final ImagePanel img_panel){
		
		// File Dialog
		final JFileChooser fc = new JFileChooser();
		fc.setCurrentDirectory(new File(respath) );
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
		
		// Tool Bar Buttons 
		JButton open_btn = new JButton("open file");// 파일 오픈
		JButton save_btn = new JButton("save file");// 파일 저장
		final JButton[] number_btns = new JButton[6];		// 에디트 버튼
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
		
		/** Key Pad Choose Number **/
		KeyboardFocusManager.getCurrentKeyboardFocusManager()
		  .addKeyEventDispatcher(new KeyEventDispatcher() {
		      public boolean dispatchKeyEvent(KeyEvent e) {
				
				int choose_number = -1;
	            
				if (e.getKeyCode() >= KeyEvent.VK_1 && e.getKeyCode() <= KeyEvent.VK_5 ) 
	            	choose_number = e.getKeyCode() - KeyEvent.VK_1;
	            else if( e.getKeyCode() >= KeyEvent.VK_NUMPAD1 && e.getKeyCode() <= KeyEvent.VK_NUMPAD5 ) 
	            	choose_number = e.getKeyCode() - KeyEvent.VK_NUMPAD1;
	            else 
	            	return false;
	            
				number_btns[choose_number + 1].doClick();
		        return false;
		      }
		});
	}
	
	private JLabel createLabel( String text ){
		JLabel ret = new JLabel();
		ret.setText(text);
		return ret;
	}
	
	private JTextField createNumberTextField( String defaultValue ){
		
		JTextField ret = new JTextField(defaultValue, 5);
		
		ret.setText(defaultValue);
		ret.setPreferredSize(new Dimension(80,20));
		ret.addKeyListener(new KeyAdapter() {
		    public void keyTyped(KeyEvent e) {
		        char c = e.getKeyChar();
		        if (!((c >= '0') && (c <= '9') ||
		           (c == KeyEvent.VK_BACK_SPACE) ||
		           (c == KeyEvent.VK_DELETE))) {
		          e.consume();
		        }
		      }
		    });
		
		return ret;
		
	}
	
	/** Chooser 선택 **/
	private ImagePanelNumberChooser fixed_chooser = new ImagePanelNumberChooser_FixedDraw();
	private ImagePanelNumberChooser free_chooser = new ImagePanelNumberChooser_FreeDraw();
	private ImagePanelNumberChooser noedit_chooser = new ImagePanelNumberChooser_NoEdit();
	
	private JPanel createFixedDraw( final ImagePanel img_panel ){
		
		JPanel panel = new JPanel();
		
		panel.setSize(200,200);
		//panel.setLayout(new GridLayout(4,1));
		SpringLayout layout = new SpringLayout();
		panel.setLayout(layout);
        
		panel.setBorder( BorderFactory.createEmptyBorder(10, 10, 10, 10));
		
		JLabel width 			= createLabel( "Width" );
		final JTextField width_t 		= createNumberTextField( "10" );
		JLabel height 			= createLabel( "Height" );
		final JTextField height_t 	= createNumberTextField( "10" );
		JButton adjust_btn		= new JButton("적용");
		
		panel.add(width);
		panel.add(width_t);
		panel.add(height);
		panel.add(height_t);
		panel.add(createLabel("") );
		panel.add(adjust_btn);
		
		//Lay out the panel.
        SpringUtilities.makeCompactGrid(panel,
                                        3, 2, //rows, cols
                                        6, 6,        //initX, initY
                                        6, 6);       //xPad, yPad
        
        // Event 
        width_t.getDocument().addDocumentListener( new DocumentListener() {
			
        	private void updateData(Document document ){
        		String cur_text = "10";
        		
        		try {
        			cur_text = document.getText(0, document.getLength() );
				} catch (BadLocationException e) {
					e.printStackTrace();
				}
        		
        		if( cur_text.equals("") ) return;
        		
        		height_t.setText(cur_text);
        	}
        	
			public void removeUpdate(DocumentEvent arg0) { updateData( arg0.getDocument() );}
			public void insertUpdate(DocumentEvent arg0) {updateData( arg0.getDocument() );}
			public void changedUpdate(DocumentEvent arg0) {updateData( arg0.getDocument() );}
		});
        
        adjust_btn.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
            	fixed_chooser.setOption("width", width_t.getText() );
            	fixed_chooser.setOption("height",height_t.getText() );
            }
        });
		
		return panel;
		
	}
	
	private void addToSideChooser(JSCAccordion accordion, final ImagePanel img_panel){
		
		accordion.setTabOrientation(TabOrientation.VERTICAL);
		
		JPanel transparentPanel = new JPanel();
		transparentPanel.setOpaque(false);
		transparentPanel.setBackground(Color.GRAY);
		
		JPanel opaquePanel = new JPanel();
		opaquePanel.setOpaque(true);
		opaquePanel.setBackground(Color.GRAY);
		
		accordion.addTab("Fixed Draw", createFixedDraw( img_panel ) );
		accordion.addTab("Free Draw", new JLabel("Is Free Draw"));
		
		// Event Process
		accordion.addAccordionListener(new AccordionListener() {
			
			public void accordionChanged(AccordionEvent accordionEvent) {
				//available fields on accordionEvent.
				
				switch (accordionEvent.getEventType()) {
				case TAB_SELECTED: {
					//add your logic here to react to a tab being selected.
					String tab = accordionEvent.getTabText();
					
					if( tab.equals("Fixed Draw") ){
						img_panel.changeChooser( fixed_chooser );
					}else if( tab.equals("Free Draw")){
						img_panel.changeChooser( free_chooser );
					}else{
						img_panel.changeChooser( noedit_chooser );
					}
					
					break;					
				}
				}
			}
		});

	}
	
	public void go() {
		
		final JFrame frame = new JFrame("odab ext verifyer");// 프레임 생성
		
		JPanel panel = new JPanel( new BorderLayout() );// 패널 생성
		
			// Image Panel
		final ImagePanel img_panel = new ImagePanel();
		
			// tool bar ( Default ) 
		JToolBar toolBar = new JToolBar("Still draggable");
		addToToolbar( toolBar, frame, img_panel );

			// arcodian panel
		JSCAccordion accordion = new JSCAccordion();
		addToSideChooser( accordion, img_panel );
		
			// panel add
		panel.add( toolBar , BorderLayout.PAGE_START );
		panel.add( accordion, BorderLayout.EAST );
		panel.add( img_panel, BorderLayout.CENTER );
		
		
		frame.getContentPane().add(panel);
		frame.setSize(800,1000);
		frame.setVisible(true);
		frame.setResizable( true );
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		// chooser ^^ 
		img_panel.changeChooser( fixed_chooser );
		
	}
	
	/**
	 * @param args
	 * @throws IOException 
	 * @throws FileNotFoundException 
	 */
	public static void main(String[] args) throws FileNotFoundException, IOException {
		new odap_ext_verifyer( args[0] ).go();
	}

}