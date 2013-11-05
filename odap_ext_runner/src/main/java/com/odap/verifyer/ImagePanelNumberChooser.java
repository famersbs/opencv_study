package com.odap.verifyer;

import java.awt.Graphics;

/**
 * Image Panel 의 숫자 선택기 인터페이스 
 * 
 * @author famersbs
 *
 */
public interface ImagePanelNumberChooser {

	/**
	 * 옵션 셋팅용
	 * @param id
	 * @param val
	 */
	void setOption( String id, String val );
	
	/**
	 * 에디트 모드 시작 !
	 * @param panel
	 * @return
	 */
	boolean startEdit( ImagePanel panel );
	
	/**
	 * 에디트 모드 종료 !
	 * @return
	 */
	boolean endEdit( );
	
	/**
	 * Edit 모드 발동시 이미지 그리기
	 * @param g
	 */
	void	drawEdit( Graphics g );
	
}
