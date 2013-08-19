Ext.define('Ext.app.view.ImagePortlet', {

    extend: 'Ext.panel.Panel',
    path_name: 'unknown',
    
    /*
    constructor: function(args) {
        if (args.path_name) {
            this.path_name = args.path_name;
        }

        return this;
    },
	*/

    initComponent: function(){
    	console.log( this.path_name );
    	
        Ext.apply(this, {
            layout: 'fit',
            width: 500,
            height: 300,
            items:[
                   { 
                   		layout:{
                   			type: 'fit',
                   			align: 'center'
                   		},
                   		items: Ext.create('Ext.Panel', {
                   										width : '100%',
                   										height : '100%',
													    layout: {
													        type: 'fit',
													        align: 'center'
													    },
													    autoScroll:true,
													    items: [
													    	{ items: Ext.create('Ext.Img', {src: 'data/getimage.jsp?name='+this.path_name+'&type=origin', width:300, height:300 } )},
                   											{ items: Ext.create('Ext.Img', {src: 'data/getimage.jsp?name='+this.path_name+'&type=facedect', width:300, height:300 } )},
                   											{ items: Ext.create('Ext.Img', {src: 'data/getimage.jsp?name='+this.path_name+'&type=binary', width:300, height:300 } )}
                   											]
													})
					}
            	]
        });

        this.callParent(arguments);
    }
});