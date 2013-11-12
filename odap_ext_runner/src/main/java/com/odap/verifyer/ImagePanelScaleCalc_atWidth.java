package com.odap.verifyer;

import java.awt.Dimension;

/**
 * 
 * 화면 높이를 기준으로 스케일을 조정 하는 녀석
 * 
 * @author famersbs
 *
 */
public class ImagePanelScaleCalc_atWidth extends ImagePanelScaleCalcer {

	private int f_width = 200;
	
	public ImagePanelScaleCalc_atWidth( int width ){
		f_width = width;
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
    		
        	// 비율 계산
        	if( f_width < image_size.getWidth() ){
        		// 세로 비율 계산
        		double w = f_width;
        		double w1 = image_size.getWidth();

        		//  calc
        		scale = w / w1;
        		
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
