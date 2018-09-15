/* displayer by Titouan Baillon */
displayer = (typeof displayer != "undefined")?displayer:{};
displayer.map = {};

displayer.map.Map = class Map extends displayer.Frame{
	constructor() {
		super();
		let that = this;
		this.element.className="card z-depth-3 grey darken-4 white-text";
		this.objects = {};
		this.classes = {Shape:displayer.map.Shape,BigRobot:displayer.map.BigRobot,SmallRobot:displayer.map.SmallRobot}
		this.bg = new Image();
		this.bg.src = "images/map.jpg";
		this.canvas = document.createElement("CANVAS");
		this.canvas.width=640;
		this.canvas.height=960;
		this.canvas.style.width="100%";
		this.canvas.style.display="block";
		this.element.appendChild(this.canvas);
		this.ctx = this.canvas.getContext("2d");
		this.ctx.globalCompositeOperation = "screen";
		this.ctx.scale(960/3000, -960/3000);
		this.ctx.translate(0, -3000);
		this.bg.onload = function(){
			that.update();
		}
	}
	addChild(){}
	removeChild(){}
	
	update(){
		if(typeof this.ctx !== "undefined" && typeof this.bg !== "undefined"){
			this.ctx.clearRect(0,0,2000,3000);
			this.ctx.drawImage(this.bg,0,0,2000,3000);
			for(let object in this.objects) this.objects[object].draw(this.ctx);
		}
	}
}

displayer.map.Shape = class Shape{
	constructor(x,y,r,color){
		
		this.x = (typeof x != "undefined")?x:0;
		this.y = (typeof y != "undefined")?y:0;
		this.r = (typeof r != "undefined")?r:0;
		this.color = (typeof color != "undefined")?color:[127,127,127];
	}
	draw(ctx){
		this.center(ctx);
		this.drawer(ctx);
		this.uncenter(ctx);
	}
	drawer(ctx){
		ctx.beginPath();
		ctx.arc(0, 0, 50, 0, 2 * Math.PI, false);
		ctx.fillStyle = "rgb("+this.color[0]+","+this.color[1]+","+this.color[2]+")";
		ctx.fill();
	}
	center(ctx){
		ctx.save();
		ctx.translate(this.x,this.y);
		ctx.rotate(this.r);
	}
	uncenter(ctx){
		ctx.restore();
	}
}

displayer.map.BigRobot = class BigRobot extends displayer.map.Shape{
	drawer(ctx){
		let inperimeter = 1200;
		let outperimeter = 1500;
		//radii
		let radii = [outperimeter/5, outperimeter/2/Math.PI, inperimeter/2/Math.PI];
		ctx.lineWidth = 10;
		ctx.shadowBlur = 10;
		for(let i=0;i<radii.length;i++){
			ctx.beginPath();
			ctx.arc(0, 0, radii[i], 0, 2 * Math.PI, false);
			ctx.shadowColor = "rgba("+this.color[0]+","+this.color[1]+","+this.color[2]+","+((i+1)/(radii.length+1))+")";
			ctx.strokeStyle = "rgba(255,255,255,"+((i+1)/(radii.length+1))+")";
			ctx.lineWidth = 10 * ((i+1)/(radii.length+1));
			ctx.stroke();
		}
		//robot
		ctx.beginPath();
		ctx.lineWidth = 10;
		ctx.strokeStyle = "white";
		ctx.fillStyle = "white";
		ctx.shadowColor = "rgb("+this.color[0]+","+this.color[1]+","+this.color[2]+")";
		ctx.strokeRect(-inperimeter/8, -inperimeter/8, inperimeter/4, inperimeter/4);
		ctx.fillRect(0, -5, inperimeter/8, 10);
	}
}
displayer.map.SmallRobot = class SmallRobot extends displayer.map.Shape{
	drawer(ctx){
		let inperimeter = 700;
		let outperimeter = 900;
		//radii
		let radii = [outperimeter/5, outperimeter/2/Math.PI, inperimeter/2/Math.PI];
		ctx.lineWidth = 10;
		ctx.shadowBlur = 10;
		for(let i=0;i<radii.length;i++){
			ctx.beginPath();
			ctx.arc(0, 0, radii[i], 0, 2 * Math.PI, false);
			ctx.shadowColor = "rgba("+this.color[0]+","+this.color[1]+","+this.color[2]+","+((i+1)/(radii.length+1))+")";
			ctx.strokeStyle = "rgba(255,255,255,"+((i+1)/(radii.length+1))+")";
			ctx.lineWidth = 10 * ((i+1)/(radii.length+1));
			ctx.stroke();
		}
		//robot
		ctx.beginPath();
		ctx.lineWidth = 10;
		ctx.strokeStyle = "white";
		ctx.fillStyle = "white";
		ctx.shadowColor = "rgb("+this.color[0]+","+this.color[1]+","+this.color[2]+")";
		ctx.strokeRect(-inperimeter/8, -inperimeter/8, inperimeter/4, inperimeter/4);
		ctx.fillRect(0, -5, inperimeter/8, 10);
	}
}