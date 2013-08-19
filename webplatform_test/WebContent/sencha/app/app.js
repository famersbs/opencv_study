var portlet_numbering = 0;

Ext.define('Ext.app.view.Portal', {
	
	    extend: 'Ext.container.Viewport',
	    requires: [
	               'Ext.app.view.PortalPanel', 
	               'Ext.app.view.PortalColumn', 
	               'Ext.app.view.ImagePortlet',
	               'Ext.app.view.ImageTree'],
	
	    getTools: function(){
	        return [{
	            xtype: 'tool',
	            type: 'gear',
	            handler: function(e, target, header, tool){
	                var portlet = header.ownerCt;
	                portlet.setLoading('Loading...');
	                Ext.defer(function() {
	                    portlet.setLoading(false);
	                }, 2000);
	            }
	        }];
	    },
	    
	    addNewPortlet: function( path_name ){
	    	
	    	++ portlet_numbering;
        	var col_1 = Ext.ComponentQuery.query('#col-1');
        	
        	var ported =  Ext.create('Ext.app.view.ImagePortlet', {path_name: path_name});
        	ported.path_name = path_name;
        	
        	// 그냥 컨테이너에 add 하면 되내? 속성으로... 대박 ~!! 
        	col_1[0].add( {
        		id: 'portlet-' + portlet_numbering,
                title: path_name +' Image',
                tools: function(){
        	        return [{
        	            xtype: 'tool',
        	            type: 'gear',
        	            handler: function(e, target, header, tool){
        	                var portlet = header.ownerCt;
        	                portlet.setLoading('Loading...');
        	                Ext.defer(function() {
        	                    portlet.setLoading(false);
        	                }, 2000);
        	            }
        	        }];
        	    },
                items: ported,
                listeners: {
                    'close': Ext.bind( this.onPortletClose, this )
                },
                
			});
        	
	    },
	
	    initComponent: function(){
	       
	        Ext.apply(this, {
	            id: 'app-viewport',
	            layout: {
	                type: 'border',
	                padding: '0 5 5 5' // pad the layout from the window edges
	            },
	            items: [{
	                id: 'app-header',
	                xtype: 'box',
	                region: 'north',
	                height: 40,
	                html: 'Ext Portal'
	            },{
	                xtype: 'container',
	                region: 'center',
	                layout: 'border',
	                items: [{
	                    id: 'app-options',
	                    title: 'Options',
	                    region: 'west',
	                    animCollapse: true,
	                    width: 400,
	                    minWidth: 150,
	                    maxWidth: 400,
	                    split: true,
	                    collapsible: true,
	                    layout:{
	                        type: 'accordion',
	                        animate: true
	                    },
	                    items: [{
	                        	id: 'navigation',
	                            title: 'image Tree',	                            
	                            items: Ext.create('Ext.app.view.ImageTree'),
	                            title: 'Simple Tree',
							    rootVisible: false
	                    		}]
	                },{
	                    id: 'app-portal',
	                    xtype: 'portalpanel',
	                    region: 'center',
	                    items: [{id: 'col-1'},{id: 'col-2'},]
	                }]
	            }]
	        });
	        this.callParent(arguments);
	    },
	
	    onPortletClose: function(portlet) {
	    	console.log("close " + portlet );
	    }
	
	});