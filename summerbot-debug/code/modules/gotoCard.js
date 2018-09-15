exports.init=function(displayer,xCom){
	exports.gotoCard = new displayer.TextCard();
	var gotoCardCol = new displayer.Col(4,6,12);
	displayer.mainFrame.addChild(gotoCardCol);
	gotoCardCol.addChild(exports.gotoCard);
	var gotoCardTitle = new displayer.Title();
	gotoCardTitle.text="Goto";
	exports.gotoCard.addChild(gotoCardTitle);
  var gotoCardRow = new displayer.Row();
  exports.gotoCard.addChild(gotoCardRow);
	exports.gotoCardInputs=["X","Y","Z"].map(t=>{
    var col = new displayer.Col(4,4,4);
    var input = new displayer.Input();
    input.text = t;
    col.addChild(input);
    return [col,input];
  });
  for(let [c,i] of exports.gotoCardInputs) gotoCardRow.addChild(c);
	var sendButton = new displayer.Button();
  sendButton.text = "send";
  sendButton.action = ()=>{xCom.send(["POS"]+exports.gotoCardInputs.map(ci=>ci[1].value),"summerbot");};
  exports.gotoCard.addChild(sendButton);
}