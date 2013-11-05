package com.odap.verifyer;

import java.awt.Graphics;

/**
 * Image Panel 의 숫자 선택기 인터페이스 
 * 
 * @author famersbs
 *
 */
public class ImagePanelNumberChooser_NoEdit implements ImagePanelNumberChooser {

	public void setOption(String id, String val) {
		
	}

	public boolean startEdit(ImagePanel panel) {
		return true;
	}

	public boolean endEdit() {
		return true;
	}

	public void drawEdit(Graphics g) {
	}
	
}
