package com.odap.verifyer;

import java.awt.Dimension;

public class ImagePanelScaleCalcer {
	
	protected double			scale = 1;
	protected Dimension		last_dim = null;
	
	
	
	/**
     * 현재 화면 사이즈와 비교하여 scale 작업을 수
     * 
     * @return true 변경됨, false 변경되지 않음
     */
    public boolean CalcScale( Dimension screen_size, Dimension image_size ){
    	
    	if( last_dim == null ) {
    		last_dim = screen_size;
    		return true;
    	}
    	return false;
    	
    }
	
	/**
     * 스케일 변형
     * @param pos
     * @return
     */
    public int getScaledpos( int pos ){
    	return (int) ( scale * pos );
    }
    
    /**
     * 실 좌표계에서 역변환
     * @param pos
     * @return
     */
    public int getreScalepos( int pos ){
    	return (int) ( pos / scale );
    }
}
