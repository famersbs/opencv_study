package com.odap.util;

import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.event.KeyEvent;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;
import java.util.Vector;

public class NADKeyBoardMan {

	/** KeyBoard Event Listner **/
	public interface KEventListener{
		void onKey( int key );
	}
	
	/** Key Event Add !!! **/
	static{
		/** Key Pad Choose Number **/
		KeyboardFocusManager.getCurrentKeyboardFocusManager()
		  .addKeyEventDispatcher(new KeyEventDispatcher() {
		      public boolean dispatchKeyEvent(KeyEvent e) {
		    	  int keycode = e.getKeyCode();
		    	  if( KeyEvent.KEY_PRESSED == e.getID() ){
			    	  synchronized( listeners ){
				    	  if( listeners.containsKey( keycode ) ){
				    		  Iterator<KEventListener> i = listeners.get(keycode).iterator();
				    		  while( i.hasNext() ){
				    			  i.next().onKey( keycode );
				    		  }
				    	  }
			    	  }
		    	  }
		        return false;
		      }
		});
	}
	
	private static Map<Integer, Vector<KEventListener> > listeners = new TreeMap<Integer, Vector<KEventListener>>();
	
	/**
	 * 키이벤트 리스너 ~!
	 * @param key
	 * @param listener
	 */
	public static void addListener( int key, KEventListener listener ){
		synchronized( listeners ){
			Vector<KEventListener> l = listeners.get( key );
			if( null == l ){
				l = new Vector<KEventListener>();
				listeners.put( key, l );
			}
			if( !l.contains( listener ) ){
				l.add( listener );
			}
		}
	}
	
	/**
	 * 
	 * @param listener
	 */
	public static void removeListener( KEventListener listener ){
		synchronized( listeners ){
			Iterator<Integer> i = listeners.keySet().iterator();
			while( i.hasNext() ){
				Integer evt = i.next();
				listeners.get( evt ).remove( listener );
			}
		}
	}
}
