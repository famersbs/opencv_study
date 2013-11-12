package com.odap.verifyer;

import java.awt.event.KeyEvent;
import java.io.File;
import java.util.Arrays;
import java.util.Map;
import java.util.TreeMap;
import java.util.Vector;

import javax.swing.JList;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import com.odap.util.NADKeyBoardMan;

public class ImageListManager implements ListSelectionListener{
	
	private JList list;
	private String respath;
	private ImagePanel panel;
	
	
	private int current_select_idx = 0;
	
	private Map<String, Integer> file_indexer = new TreeMap<String, Integer>();
	private Vector<String> file_list = new Vector<String>();
	
	public ImageListManager( JList list, String respath, ImagePanel panel ){
		this.list = list;
		this.respath = respath;
		this.panel = panel;
		
		// KeyEvent ON!!
		enableKeyEvent();
	}
	
	private void enableKeyEvent(){
		// Key Event 등록
		NADKeyBoardMan.KEventListener listner = new NADKeyBoardMan.KEventListener() {
			public void onKey(int key) {
				int inc_dir = (key == KeyEvent.VK_LEFT ? -1 : 1 );
				setSelectImage( current_select_idx + inc_dir );
				
				
			}
		};
		
		NADKeyBoardMan.addListener(KeyEvent.VK_LEFT, listner );
		NADKeyBoardMan.addListener(KeyEvent.VK_RIGHT, listner );
		
	}
	
	/**
	 * 지정된 인덱스를 수정 한다.
	 */
	private void changeSelectImage( int index ){
		// first Save
    	panel.saveXML();
    	
    	refreshCheck( current_select_idx );
    	
    	// Find out which indexes are selected.
    	current_select_idx = index;
    	
    	// second load
    	panel.loadImage( respath + "/" + getImageName( current_select_idx ) );
	}
	
	/*
	 * 
	 */
	private void setSelectImage( int index ){
		if( index < 0 || index >= file_list.size() ){
			return;
		}
		else{
			System.out.println("TEST " + index);
			list.setSelectedIndex( index );
			changeSelectImage( index );
		}
	}
	
	/**
	 * Load
	 */
	public void load(){
		
		// Step 1. File List Load
		File resfp = new File( respath );
		
		String[] filelist = resfp.list();
		
		Arrays.sort(filelist);
		
		// Step 2. Filtering 
		int current_cnt = 0;
		for( int i = 0 ; i < filelist.length ; ++ i ){
			
			if( 3 > filelist[i].length() ) continue;
			String extension = filelist[i].substring( filelist[i].length() - 3 );
			System.out.println( extension );
			if( extension.equals("jpg") ||
				extension.equals("gif") ||
				extension.equals("png")
			){
				// It's a image
				file_list.add( filelist[i] );
				file_indexer.put( filelist[i] ,  current_cnt );
				++ current_cnt;
				
			}else if ( extension.equals("xml") ){
				// It's a XML
				String imgname = filelist[i].substring(0, filelist[i].length() - 4 );
				
				Integer cur_i = file_indexer.get( imgname );
				
				if( cur_i >= 0 ) {
					file_list.set( cur_i, "[V] " + file_list.get( cur_i ) );
				}
				
			}
		}
		
		list.setListData( file_list );
		
		// First Index;
		list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		list.setSelectedIndex( current_select_idx );

		// Selection Model 
		list.getSelectionModel().addListSelectionListener( this );
		
		// First load 
		panel.loadImage( getFullImagePath( current_select_idx ) );

	}

	public void valueChanged(ListSelectionEvent e) {
		
		ListSelectionModel lsm = (ListSelectionModel)e.getSource();

        boolean isAdjusting = e.getValueIsAdjusting();
        
        if( isAdjusting ){
        	
	        if (lsm.isSelectionEmpty()) {
	        	System.out.println(" <none>");
	        } else {
	        	changeSelectImage( lsm.getMinSelectionIndex() );
	        }
	        
        }
		
	}
	

	
	/**
	 * 
	 * @param current_select_idx2
	 */
	private void refreshCheck(int i) {
		
		// check is verify
		String filename = file_list.get( i );
		
		if( filename.indexOf( "[V] " ) < 0 ){
			
			// get file name
			String fullpath = getFullImagePath( i );
			
			// xml file find
			boolean isverify = new File( fullpath + ".xml" ).isFile();
			
			if( isverify ){
				file_list.set( i , "[V] " + filename );
			}
		}
		
		
	}

	/**
	 * Image Name Create 
	 * @param i
	 * @return
	 */
	private String getImageName(int i) {
		String file_name = file_list.get( i );
		
		// 체크 된 녀석인지 확인
		if( file_name.indexOf("[V] ") >= 0 ) {
			System.out.println(" image name v : "  + file_name.substring( 4 ) );
			return file_name.substring( 4 );
		}
		System.out.println(" image name : "  + file_name );
		return file_name;
	}
	
	/**
	 * 
	 * @param i
	 * @return
	 */
	private String getFullImagePath( int i ){
		
		return respath + "/" + getImageName( i );
		
	}

}
