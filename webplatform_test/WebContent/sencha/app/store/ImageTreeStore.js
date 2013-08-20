Ext.define('Ext.app.store.ImageTreeStore', {

	extend: 'Ext.data.TreeStore',
    model: 'Ext.app.model.ImageTreeModel',
    //autoSync: true,

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
    root: {
        text: "Divisions",
        expanded: true,
        id: 'NULL'
    },
	proxy: {
        type: 'ajax',
        api: {
            read: '../data/filetree.jsp'
        },
        reader: {
            type: 'json',
            root: 'divisions'
        }
    }
});