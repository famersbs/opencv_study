Ext.define('Ext.app.view.ImageTree', {

	extend: 'Ext.tree.Panel',
    xtype: 'basic-trees',
    layout: {
    	type: 'vbox', 
    	pack: 'start', 
    	align: 'stretch', 
        tdAttrs: { style: 'padding: 10px;' }
    },
    
    frameHeader : false,
    header : false,
    
    autoWidth: true,
    autoHeight: true,
    
    dockedItems: [{
        xtype: 'toolbar',
        items: [ {
            text: 'Add Image',
            handler: function() {
            	Ext.create('Ext.app.view.AddImagePopupWindow',{ testListen: function(){ alert("is_test"); } } ).show();//Ext.app.AddImagePopupWindow
            }
        }]
    }],    
    
    initComponent: function() {
    	
    	var store = Ext.create( 'Ext.app.store.ImageTreeStore', {} );
    	Ext.apply(this, {
    	    store: store,
    	    hideHeaders : true,
    	    collapsible: true,
            useArrows: true,
            rootVisible: false,
            store: store,
            multiSelect: true,
            singleExpand: true,
    	    columns: [{
                xtype: 'treecolumn', //this is so we know which column will show the tree
                text: 'Task',
                flex: 1,
                sortable: true,
                dataIndex: 'name'
            }],
            listeners:{
            	itemdblclick: function ( obj, record, item, index, e, eOpts ){
            		global_portal_port.addNewPortlet( record.get("name") );
            	}
            }
        });
    	
        this.callParent(arguments);
        
        imgtree_store = store;
    }
    
});