//AddImagePopupWindow

Ext.define('Ext.app.view.AddImagePopupWindow', {

	extend: 'Ext.window.Window',
    
	width: 640,    
	title: 'Add Image',
	height: 200,
	layout: 'fit', 	    
	
   initComponent: function() {
	   
	   var popup_window = this;

	   // 파일 업로드
       var panel = new Ext.form.FormPanel({
						            width:'100%',
						            frame:false,
						            style : 'padding:10px',
						            bodyPadding: 10,
						            enctype : 'multipart/form-data',
						            items: [
											{
											    xtype: 'textfield',
											    name: 'name',
											    fieldLabel: 'name',
											    style : 'padding:10px',
											    labelWidth: 50,
											    msgTarget: 'side',
											    allowBlank: false,
											    anchor: '100%'
											},
						                    {
								                xtype: 'filefield',
								                name: 'photo',
								                fieldLabel: 'Photo',
								                style : 'padding:10px',
								                labelWidth: 50,
								                msgTarget: 'side',
								                allowBlank: false,
								                anchor: '100%',
								                buttonText: 'Select Photo...'
								            }
						            ],
						            buttons: [{
						                text: 'Upload',
						                handler: function() {
						                    var form = this.up('form').getForm();
						                    if(form.isValid()){
						                        form.submit({
						                            url: 'data/uploadimage.jsp',
						                            waitMsg: 'Uploading your photo...',
						                            method: 'POST',
						                            enctype : 'multipart/form-data',
						                            success: function(fp, o) {
						                                
						                            	Ext.Msg.alert('Success', action.result.msg);
						                            	//imgtree_store.sync();
						                            	
						                            	imgtree_store.reload();
														
						                            	popup_window.close();
						                            },
						                            failure: function(fp, action ){
						                            	if (action.result.status == true) {
						                            		Ext.Msg.alert('Success', action.result.msg);
						                                }else{						                            	
						                                	//Ext.Msg.alert('Failed', action.result.msg);
						                                }
						                                
						                                imgtree_store.reload();
														
						                            	popup_window.close();
						                            }
						                        });
						                    }
						                }
						            }]
						         });
		
        Ext.apply(this, 
        		{
        			items:panel,
        			resizable: false
        			
        		}
        );
        		
        		
        this.callParent(arguments);
    },
    
    modal:true		// 이거만 사용할 수 있도록합니다.
});