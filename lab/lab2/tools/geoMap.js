/*
Author: hwd

Date:2017-03-07

Description:Provide  GeoMap
 */
function GeoMap(divId,zoom,baseMapType){
	BaseMap.call(this,divId,zoom,baseMapType);
	this.geoJsonLayer=null;
	this.toolTipLayerGroup=null;
	this.overlayMaps=null;
}
GeoMap.prototype=new BaseMap();
GeoMap.prototype.display=function(){
	this.dataConversion();
	this.center=this.getCentroid(geometry.features !== undefined?geometry.features:[geometry]);
	this.geoJsonLayer=this.getGeoJsonLayer();
	this.tooltipLayerGroup=this.getToolTipLayer();
    let overlayMaps;
	this.overlayMaps={
		"Results":this.geoJsonLayer,
		"Description":this.tooltipLayerGroup
	};
	this.map=this.getMap({divId:this.divId,center:this.center.reverse(),zoom:this.zoom,layers:[this.baseMaps[(this.num2Type())[this.baseMapType]],this.geoJsonLayer]});
    L.control.layers(this.baseMaps, this.overlayMaps).addTo(this.map);
};