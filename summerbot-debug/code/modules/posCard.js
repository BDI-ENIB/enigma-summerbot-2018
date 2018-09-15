exports.init=function(displayer,xCom){
	exports.placementCard = new displayer.TextCard();
	var placementCardCol = new displayer.Col(4,6,12);
	displayer.mainFrame.addChild(placementCardCol);
	placementCardCol.addChild(exports.placementCard);
	var placementCardTitle = new displayer.Title();
	placementCardTitle.text="Position";
	exports.placementCard.addChild(placementCardTitle);
	exports.placementCardRows=[];
	var placementCardActions=[
		v=>{xCom.send(["SPOS",v,exports.placementCardRows[1][0].value,exports.placementCardRows[2][0].value],"summerbot")},
		v=>{xCom.send(["SPOS",exports.placementCardRows[0][0].value,v,exports.placementCardRows[2][0].value],"summerbot")},
		v=>{xCom.send(["SPOS",exports.placementCardRows[0][0].value,exports.placementCardRows[1][0].value,v],"summerbot")}
	];
	function makeDataRow(parent,t,action){
		var row = new displayer.Row();
			parent.addChild(row);
		var text = new displayer.Input();
			var textcol = new displayer.Col(4,4,4);
			row.addChild(textcol);
			textcol.addChild(text);
			text.value=0;
			text.disabled=true;
		var input = new displayer.Input();
			var inputcol = new displayer.Col(4,4,4);
			row.addChild(inputcol);
			inputcol.addChild(input);
			input.text=t;
		var button = new displayer.Button();
			button.text="update";
			button.action=()=>{action(isNaN(parseFloat(input.value))?parseFloat(text.value):parseFloat(input.value));};
			var buttoncol = new displayer.Col(4,4,4);
			row.addChild(buttoncol);
			buttoncol.addChild(button);
		return [text,input,button];
	}
	var texts=["X","Y","A"];
	for(var i=0;i<3;i++){
		exports.placementCardRows.push(makeDataRow(exports.placementCard,texts[i],placementCardActions[i]));
	}
}
exports.place=function(x,y,r){
	exports.placementCardRows[0][0].value = x;
	exports.placementCardRows[1][0].value = y;
	exports.placementCardRows[2][0].value = r;
}