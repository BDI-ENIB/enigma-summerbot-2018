exports.init=function(displayer,xCom){
  exports.xCom=xCom;
	exports.card = new displayer.TextCard();
	var cardCol = new displayer.Col(4,6,12);
	displayer.mainFrame.addChild(cardCol);
	cardCol.addChild(exports.card);
	var cardTitle = new displayer.Title();
	cardTitle.text="logger";
	exports.card.addChild(cardTitle);
  exports.text = new displayer.Text();
  exports.text.text="";
	exports.card.addChild(exports.text);
}
exports.registerCommands = function(){
  exports.xCom.addCommand("LOG",(com,con)=>{
		exports.text.text += com[1].replace(/_/g," ")+"<br>";
	});
}