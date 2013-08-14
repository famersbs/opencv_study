package test;


import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

/**
 * Application Lifecycle Listener implementation class sencha_test_listner
 *
 */
public class sencha_test_listner implements ServletContextListener {

    /**
     * Default constructor. 
     */
    public sencha_test_listner() {
        // TODO Auto-generated constructor stub
    }

	/**
     * @see ServletContextListener#contextInitialized(ServletContextEvent)
     */
    public void contextInitialized(ServletContextEvent event) {
    	
    	 ServletContext scObj	= event.getServletContext();
    	 imagestore_root_path 	= scObj.getInitParameter("imagestore_root_path");
    	 process				= scObj.getInitParameter("process");
    	 System.out.println( imagestore_root_path );
    }

	/**
     * @see ServletContextListener#contextDestroyed(ServletContextEvent)
     */
    public void contextDestroyed(ServletContextEvent arg0) {
        // TODO Auto-generated method stub
    }
	
    
    public static String imagestore_root_path = "";
    public static String process = "";
    
}
