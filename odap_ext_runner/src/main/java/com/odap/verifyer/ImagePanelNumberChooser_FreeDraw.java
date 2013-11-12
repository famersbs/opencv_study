package com.odap.verifyer;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

public class ImagePanelNumberChooser_FreeDraw implements ImagePanelNumberChooser, MouseMotionListener, MouseListener {
	private ImagePanel target = null;
	
	/**
	 * 
	 */
	public ImagePanelNumberChooser_FreeDraw(){
		
	}
	

	public void setOption(String id, String val) {		
	}

	public boolean startEdit(ImagePanel panel) {
		target = panel;
		
		target.addMouseListener(this);
		target.addMouseMotionListener(this);
    	
		return true;
	}

	public boolean endEdit() {
		
		target.removeMouseListener( this );
		target.removeMouseMotionListener( this );
		
		return true;
	}

	public void drawEdit(Graphics g) {
		
		// put editable mode
    	if( edit_enter ){
    		g.setColor( new Color( 0xFF0000) );
    		// 저장 시에는 스케일에 맞게 저장 되어야 한다.
    		g.drawRect( edit_rect.x , 
    					edit_rect.y ,
    					edit_rect.width ,
    					edit_rect.height );
    	}
		
	}
	
	
	/** Mouse Processe **/
    // 마우스 처리
    private boolean 	edit_enter = false;
    private Point		edit_spt = new Point();
    private Rectangle 	edit_rect = new Rectangle();
    
	public void mousePressed(MouseEvent e) {
		
		edit_spt.x = edit_rect.x = e.getX();
		edit_rect.width = 1;
		
		edit_spt.y = edit_rect.y = e.getY();
		edit_rect.height = 1;
		
		edit_enter = true;
		
		// Target Repaint
		target.repaint();
	}

	public void mouseReleased(MouseEvent e) {
		
		edit_enter = false;
		
		Rectangle rect = new Rectangle( );
		
		rect.x		 	= target.getreScalepos( edit_rect.x );
		rect.y		 	= target.getreScalepos( edit_rect.y );
		rect.width	 	= target.getreScalepos( edit_rect.width );
		rect.height		= target.getreScalepos( edit_rect.height );
		
		target.setNumberRect( rect );
		
		target.repaint();
		
	}
	
	public void mouseDragged(MouseEvent e) {
	
		if( edit_spt.x < e.getX() ){
			edit_rect.width = e.getX() - edit_rect.x;
			edit_rect.x = edit_spt.x;
		}
		else{
			edit_rect.x = e.getX();
			edit_rect.width = edit_spt.x - e.getX();
		}
		
		if( edit_spt.y < e.getY() ){
			edit_rect.height = e.getY() - edit_rect.y;
			edit_rect.y = edit_spt.y;
		}
		else{
			edit_rect.y = e.getY();
			edit_rect.height = edit_spt.y - e.getY();
		}
		
		target.repaint();
		
	}

	public void mouseMoved(MouseEvent e) {}
	public void mouseClicked(MouseEvent e) {}
	public void mouseEntered(MouseEvent e) {}
	public void mouseExited(MouseEvent e) {}
	
}
