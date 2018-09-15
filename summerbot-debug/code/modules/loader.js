exports.init=function(displayer){
	exports.loader = new displayer.TextCard();
	var loaderCol = new displayer.Col(12,12,12);
	displayer.mainFrame.addChild(loaderCol);
	loaderCol.addChild(exports.loader);
	var loaderTitle = new displayer.Title();
	loaderTitle.text="Awaiting devices";
	exports.loader.addChild(loaderTitle);
	var loaderTooLong = new displayer.Text();
	loaderTooLong.text="sync is taking very long. There might be an issue.";
	loaderTooLong.hidden=true;
	exports.loader.addChild(loaderTooLong);
	setTimeout(()=>{loaderTooLong.hidden=false},10000);
}
exports.hide=function(){
	exports.loader.hidden=true;
}
exports.show=function(){
	exports.loader.hidden=false;
}