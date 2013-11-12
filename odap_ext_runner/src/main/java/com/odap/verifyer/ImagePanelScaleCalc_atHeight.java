package com.odap.verifyer;

import java.awt.Dimension;

/**
 * 
 * 화면 높이를 기준으로 스케일을 조정 하는 녀석
 * 
 * @author famersbs
 *
 */
public class ImagePanelScaleCalc_atHeight extends ImagePanelScaleCalcer {
	
	public ImagePanelScaleCalc_atHeight(){
		
	}
	
	/**
     * 현재 화면 사이즈와 비교하여 scale 작업을 수
     * 
     * @return true 변경됨, false 변경되지 않음
     */
	@Override
    public boolean CalcScale( Dimension screen_size, Dimension image_size ){
    	
    	// 사이즈가 변경 되었을 경우 계산을 다시 한다.
    	if( null == last_dim || last_dim.equals( screen_size ) ){
    		// scale 처리
        	Dimension size = screen_size;
        	
        	// 비율 계산
        	if( size.height < image_size.getHeight() ){
        		// 세로 비율 계산
        		double h = size.height;
        		double h1 = image_size.getHeight();

        		//  calc
        		scale = h / h1;
        		
        		return true;
        		
        	}else{
        		scale = 1;
        		return true;
        	}
        	
    	}else{
    		return false;
    	}
    	
    }
    
}
