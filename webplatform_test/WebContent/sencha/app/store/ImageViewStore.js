Ext.define('Ext.app.store.ImageViewStore', {

	extend: 'Ext.data.Store',
    model: 'Ext.app.model.ImageTreeModel',
    autoLoad: false,

    listeners: {
        load:function(){
            console.log('Schemes Data store loaded ' + this.fields );
        },
        
        append: function( thisNode, newChildNode, index, eOpts ){
        	if( !newChildNode.isRoot() ){
        		newChildNode.set('leaf', true );
        	}
        }
    },
    
	proxy: {
        type: 'ajax',
        api: {
            read: 'data/imageinfo.jsp'
        },
        reader: {
            type: 'json'
        }
    },
    
    loadImageInfo: function( img_name ){
    	this.load({
		    	    params: {
		    	        name: img_name,
		    	        type: 'user'
		    	    },
		    	    callback: function(records, operation, success) {
		    	        // do something after the load finishes
		    	    },
		    	    scope: this
		    	});
    }
});