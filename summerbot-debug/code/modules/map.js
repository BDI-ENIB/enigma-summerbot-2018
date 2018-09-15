exports.init=function(displayer){
	exports.map = new displayer.map.Map();
	var mapcol = new displayer.Col(4,6,12);
	displayer.mainFrame.addChild(mapcol);
	mapcol.addChild(exports.map);
}
exports.update=function(){
	exports.map.update();
}
exports.connect=function(id,object){
	exports.map.objects[id]=object;
}
