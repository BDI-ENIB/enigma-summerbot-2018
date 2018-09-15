var XCom = require("./backend-libs/XCom");
var mapModule = require("./modules/map");
var loaderModule = require("./modules/loader");
var myBotModule = require("./modules/myBot");
var posCardModule = require("./modules/posCard");
var gotoCardModule = require("./modules/gotoCard");
var motionCardModule = require("./modules/motionCard");
var checkCardModule = require("./modules/checkCard");
var paramCardModule = require("./modules/paramCard");
var loggerCardModule = require("./modules/loggerCard");

var xCom = new XCom.XCom();

// ===== DISPLAY ===== //
//loader
loaderModule.init(displayer);
//map
mapModule.init(displayer);
//Position
posCardModule.init(displayer,xCom);
//manual
//motionCardModule.init(displayer,xCom);
//Goto
//gotoCardModule.init(displayer,xCom);
//Checks
//checkCardModule.init(displayer,xCom);
//parameters
paramCardModule.init(displayer,xCom);
//logger
loggerCardModule.init(displayer,xCom);
//for(let e of displayer.mainFrame.children) e.element.style.height ="300px"; //ugly hack to correct an uglier bug

// ===== EVENTS ===== //
var readyPromise = Promise.all([xCom.whenConOpen("summerbot")]);


// ===== ACTIONS ===== //

//debug
readyPromise.then(()=>{
	xCom.addCommand("SCRAMBLED",(com,con)=>{
		alert("scrambled");
	})
});

//loader dismissal
readyPromise.then(()=>{
	console.log("all devices ready");
	loaderModule.hide(); // hide loader
});

// bigRobot update
myBotModule.init(displayer,mapModule,false);
readyPromise.then(()=>{
	xCom.addCommand("POS",(com,con)=>{
		myBotModule.place(parseFloat(com[1]),parseFloat(com[2]),parseFloat(com[3]));
		mapModule.update();
	});
});

//position
readyPromise.then(()=>{
	xCom.addCommand("POS",(com,con)=>{
		posCardModule.place(com[1],com[2],com[3]);
	})
});
//checks
readyPromise.then(()=>{
	//checkCardModule.registerCommands();
});
//parameters
readyPromise.then(()=>{
  paramCardModule.registerCommands();
});
//logger
readyPromise.then(()=>{
  loggerCardModule.registerCommands();
});
//launch periodics
readyPromise.then(()=>{
	//setTimeout(()=>xCom.send(["START"],"summerbot"),500);
	setTimeout(()=>xCom.send(["+POS"],"summerbot"),2000);
});