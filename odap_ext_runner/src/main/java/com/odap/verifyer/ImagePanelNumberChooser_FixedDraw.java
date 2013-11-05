package com.odap.verifyer;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

public class ImagePanelNumberChooser_FixedDraw implements ImagePanelNumberChooser, MouseMotionListener, MouseListener {
	private ImagePanel target = null;
	
	private int cur_mouse_x		= 0;
	private int cur_mouse_y		= 0;
	private int chooseWidth 	= 10;
	private int chooseHeight	= 10;
	
	
	/**
	 * 
	 */
	public ImagePanelNumberChooser_FixedDraw(){
		
	}
	

	public void setOption(String id, String val) {		
		
		try{
			if( "width".equals( id ) ){
				chooseWidth = Integer.parseInt( val );
			}else if ("height".equals( id ) ){
				chooseHeight = Integer.parseInt( val );
			}
		}catch(Exception ex ){
			
		}
		
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
		g.setColor( new Color( 0xFF0000) );
		// 저장 시에는 스케일에 맞게 저장 되어야 한다.
		g.drawRect( target.getScaledpos( cur_mouse_x ), 
					target.getScaledpos( cur_mouse_y ),
					target.getScaledpos( chooseWidth ),
					target.getScaledpos( chooseHeight ) );
	}
	
	
	/** Mouse Processe **/
    // 마우스 처리
	public void mousePressed(MouseEvent e) {
	}

	public void mouseReleased(MouseEvent e) {
	}
	
	public void mouseDragged(MouseEvent e) {
	}

	public void mouseMoved(MouseEvent e) {
		
		cur_mouse_x = e.getX();
		cur_mouse_y = e.getY();
		
		// Target Repaint
		target.repaint();
	}
	public void mouseClicked(MouseEvent e) {
		
		Rectangle rect = new Rectangle( );
		
		rect.x		 	= target.getreScalepos( cur_mouse_x );
		rect.y		 	= target.getreScalepos( cur_mouse_y );
		rect.width	 	= target.getreScalepos( chooseWidth );
		rect.height		= target.getreScalepos( chooseHeight );
		
		target.setNumberRect( rect );
		
		
	}
	public void mouseEntered(MouseEvent e) {}
	public void mouseExited(MouseEvent e) {}

}
