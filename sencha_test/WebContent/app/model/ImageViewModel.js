Ext.define('Ext.app.model.ImageViewModel', {
	extend: 'Ext.data.Model',
    fields: ['name','normal_url', 'facedetect_url', 'binary_url', 'layout_url', 'ocr_text']
});