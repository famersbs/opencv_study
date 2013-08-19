/**
 * @class Ext.app.view.PortalColumn
 * @extends Ext.container.Container
 * A layout column class used internally be {@link Ext.app.view.PortalPanel}.
 */
Ext.define('Ext.app.view.PortalColumn', {
    extend: 'Ext.container.Container',
    alias: 'widget.portalcolumn',

    requires: [
        'Ext.layout.container.Anchor',
        'Ext.app.view.Portlet'
    ],

    layout: 'anchor',
    defaultType: 'portlet',
    cls: 'x-portal-column'

    // This is a class so that it could be easily extended
    // if necessary to provide additional behavior.
});