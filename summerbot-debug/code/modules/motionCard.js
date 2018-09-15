exports.init=function(displayer,xCom){
	exports.card = new displayer.TextCard();
	var cardCol = new displayer.Col(4,6,12);
	displayer.mainFrame.addChild(cardCol);
	cardCol.addChild(exports.card);
	var cardTitle = new displayer.Title();
	cardTitle.text="Manual";
	exports.card.addChild(cardTitle);
  var row = new displayer.Row();
  exports.card.addChild(row);
	exports.buttons=[
    ["backward 10",["TRANS",-10]],
    ["backward 1",["TRANS",-1]],
    ["forward 10",["TRANS",10]],
    ["forward 1",["TRANS",1]],
    ["left 45",["ROT",Math.PI/4]],
    ["left 5",["ROT",Math.PI/180*5]],
    ["right 45",["ROT",-Math.PI/4]],
    ["right 5",["ROT",-Math.PI/180*5]],
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