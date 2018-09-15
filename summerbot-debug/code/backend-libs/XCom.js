const EventEmitter = require('events');

var convertStringToArrayBuffer=function(str) {
	var buf=new ArrayBuffer(str.length);
	var bufView=new Uint8Array(buf);
	for (var i=0; i<str.length; i++) {
		bufView[i]=str.charCodeAt(i);
	}
	return buf;
}
function convertArrayBufferToString(buf) {
	return String.fromCharCode.apply(null, new Uint8Array(buf));
}

class XCom{
	constructor(){
		var that=this;
		this.ports=[];
		this.names={};
		this.eventEmitter = new EventEmitter();
		console.log("go");
		
		this.refresh();
		
		this.onRefresh = ()=>{
			that.lines={};
			for(var i in that.ports){
				that.lines[that.ports[i]]="";
				that.send(["AYR"],that.ports[i]); //are you ready
			}
		};//(re)create the line pool
		
		this.onReady = ()=>{
			chrome.serial.onReceive.addListener(function(info){
				if (info.data) {
					var str = convertArrayBufferToString(info.data);
					that.lines[info.connectionId] += str;
					var buffer = that.lines[info.connectionId].split("\n");
					while (buffer.length>1) {
						var c=buffer.shift().replace("\r","").split(" ");
						that.commandInput(c,info.connectionId);
						that.lines[info.connectionId] = buffer.join("\n");
					}
				}
			}); 
		};//line reading and buffering
	}
	async refresh(){
		var ports = await new Promise((res,rej)=>chrome.serial.getDevices(res));
		console.log(ports);
		var portsCon = await Promise.all(
			ports.map(port=>new Promise((res,rej)=>chrome.serial.connect(port.path, {bitrate:port.productId==1155?250000:9600}, res)))
		);// connect to all ports
		this.ports=portsCon.map((i)=>i.connectionId);// filter id
		console.log(this.ports);
		this.eventEmitter.emit('refresh',true);
	}
	commandInput(com,con){
		console.log(com);
		if(com.length<1) return;
		if(com[0]=="READY"){
			this.send(["WHOIS"],con);
		}
		if(com[0]=="IAM"){
			this.names[com[1]]=con;
			this.eventEmitter.emit('conOpen-'+com[1],true);
		}
		var name=con;
		for(var k in this.names){
			if(this.names[k]==con){
				name=k;
			}
		}
		con=name;
		this.eventEmitter.emit('command',com,con);
		this.eventEmitter.emit('command-'+com[0],com,con);
	}
	send(com,con){
		con=(typeof con == "undefined")?"*":con;
		if(typeof con == typeof "s"){
			if(con=="*" || con==""){
				for(i in this.ports) this.send(com,this.ports[i]);
				return;
			}
			else if(con in this.names)con=this.names[con];
		}
		console.log("sending...");
		chrome.serial.send(con, convertStringToArrayBuffer(com.join(" ")+"\n"), ()=>{console.log("sent "+com.join(" "));});
	}
	set onRefresh(fun){this.eventEmitter.on('refresh',()=>{setImmediate(() => {fun();});})}
	set onReady(fun){this.eventEmitter.once('refresh',()=>{setImmediate(() => {fun();});})}
	addCommand(comName,fun){
		if(typeof comName == "undefined" || comName=="" || comName=="*")this.eventEmitter.on('command',(com,con)=>{setImmediate(() => {fun(com,con);});});
		else this.eventEmitter.on('command-'+comName,(com,con)=>{setImmediate(() => {fun(com,con);});});
	}
	whenConOpen(comName){
		return new Promise((res,rej)=>{this.eventEmitter.once('conOpen-'+comName,()=>{setImmediate(() => {res();});})});
	}
}
exports.XCom=XCom;