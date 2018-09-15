/* displayer by Titouan Baillon */
displayer = {}
displayer.Frame = class Frame{
	constructor(){
		this.children = [];
		this.element = document.createElement("DIV");
	}
	addChild(child){
		this.children.push(child);
		this.element.appendChild(child.element);
	}
	removeChild(child){
		var id=this.children.indexOf(child);
		if (id > -1) {
			this.children.splice(id, 1);
		}
		this.element.removeChild(child.element);
	}
	get hidden(){
		return this.element.style.display=="none";
	}
	set hidden(h){
		if(h)this.element.style.display="none";
		else this.element.style.display="";
	}
}
displayer.Row = class Row extends displayer.Frame{
	constructor(){
		super();
		this.element.className="row";
	}
}
displayer.Col = class Col extends displayer.Frame{
	constructor(l,m,s){
		super();
		this.element.className="col"
			+((typeof l != "undefined")?(" l"+l):"")
			+((typeof m != "undefined")?(" m"+m):"")
			+((typeof s != "undefined")?(" s"+s):"");
	}
}
displayer.TextCardContent = class TextCardContent extends displayer.Frame{
	constructor(){
		super();
		this.element.className="card-content";
	}
}
displayer.TextCard = class TextCard extends displayer.Frame{
	constructor(){
		super();
		this.element.className="card z-depth-3 grey darken-4 white-text";
		this.content = new displayer.TextCardContent();
		this.element.appendChild(this.content.element);
	}
	addChild(child){
		this.content.addChild(child);
	}
	removeChild(child){
		this.content.removeChild(child);
	}
}
displayer.Text = class Text extends displayer.Frame{
	constructor(){
		super();
		this.element.textContent="";
	}
	addChild(){}
	removeChild(){}
	get text(){
		return this.element.innerHTML;
	}
	set text(t){
		this.element.innerHTML=t;
	}
}
displayer.Title = class Title extends displayer.Frame{
	constructor(){
		super();
		this.element.className="card-title light-blue-text text-darken-3";
		this.element.textContent="";
	}
	addChild(){}
	removeChild(){}
	get text(){
		return this.element.textContent;
	}
	set text(t){
		this.element.textContent=t;
	}
}
displayer.Image = class Image extends displayer.Frame{
	constructor(){
		super();
		this.element = document.createElement("IMG");
		this.element.style.width="100%";
		this.element.src="";
	}
	addChild(){}
	removeChild(){}
	get src(){
		return this.element.src;
	}
	set src(s){
		this.element.src=s;
	}
}
displayer.Button = class Button extends displayer.Frame{
	constructor(){
		super();
		this.element.style.width="100%";
		this.element.style.padding="0";
    this.element.style.margin ="1px"
		this.element.className="btn light-blue darken-3";
		this.element.textContent="";
	}
	addChild(){}
	removeChild(){}
	get text(){
		return this.element.textContent;
	}
	set text(t){
		this.element.textContent=t;
	}
	get action(){
		return this.element.onclick;
	}
	set action(f){
		this.element.onclick=f;
	}
}
displayer.Input = class Input extends displayer.Frame{
	constructor(){
		super();
		this.element = document.createElement("INPUT");
		this.element.style.width="100%";
		this.element.style.margin="0";
		this.element.style.color="white";
		this.element.placeholder="";
	}
	addChild(){}
	removeChild(){}
	get text(){
		return this.element.placeholder;
	}
	set text(t){
		this.element.placeholder=t;
	}
	get value(){
		return this.element.value;
	}
	set value(t){
		this.element.value=t;
	}
	get disabled(){
		return this.element.readOnly;
	}
	set disabled(t){
		this.element.readOnly=t;
	}
}
displayer.mainFrame = new displayer.Row();
displayer.mainFrame.element.className += " blue-grey darken-4";
displayer.mainFrame.element.style.minHeight += "100vh";
displayer.mainFrame.element.style.margin += "0";
document.body.appendChild(displayer.mainFrame.element);