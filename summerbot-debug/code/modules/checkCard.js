exports.init=function(displayer,xCom){
  exports.xCom=xCom;
	exports.card = new displayer.TextCard();
	var cardCol = new displayer.Col(4,6,12);
	displayer.mainFrame.addChild(cardCol);
	cardCol.addChild(exports.card);
	var cardTitle = new displayer.Title();
	cardTitle.text="selfCheck";
	exports.card.addChild(cardTitle);
  var row = new displayer.Row();
  exports.card.addChild(row);
	exports.buttons=[
    ["Claw",["CHECK","CLAW"]],
    ["Motion",["CHECK","MOTION"]],
    ["Bee",["CHECK","BEE"]],
    ["Starter",["CHECK","STARTER"]],
    ["Sensors",["CHECK","SENSORS"]],
    ["Display",["CHECK","DISPLAY"]],
  ].map(d=>{
    var col = new displayer.Col(6,6,6);
    var button = new displayer.Button();
    button.text = d[0];
    button.action = ()=>{xCom.send(d[1],"summerbot");};
    col.addChild(button);
    return [col,button];
  });
  for(let [c,b] of exports.buttons) row.addChild(c);
}
exports.registerCommands = function(){
  exports.xCom.addCommand("CHECK",(com,con)=>{
		alert("selfCheck results:\n"+com.join("\n"));
	});
}