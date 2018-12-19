//http://dmitrybaranovskiy.github.io/raphael/
//https://dmitrybaranovskiy.github.io/raphael/reference.html
//https://mech.fsv.cvut.cz/~stransky/en/software/raphaeltools/
function slider(opts,onchange){
	opts	= opts|| {};		
	const slider_id_ = document.getElementById(opts.container);
	var min=opts.min|| 0;
	var max=opts.max|| 100;	
	var value=opts.value|| min;
	var paper = Raphael(slider_id_);	
	var min_y;
	var max_y;
	var onchange_=onchange;

	function start() {
	    this.oy = this.attr("cy");
	};
	function move(dx, dy, mx, my, ev) {
		let new_y=(this.oy + dy)
		if(new_y>max_y){
			new_y=max_y;
		}
		if(new_y<min_y){
			new_y=min_y;
		}
		this.attr({ cy:new_y });
		value=min+(new_y-min_y)/(max_y-min_y)*(max-min);
		if(onchange_)
			onchange_(value);
	};
	function end() {
	};
	function get_coord_y(){
		return min_y+(value-min)/(max-min)*(max_y-min_y);
	}
	this.resize=function(){
		paper.setSize(slider_id_.offsetWidth ,slider_id_.offsetHeight );
		paper.clear();
		min_y=paper.width/2+1;//radius;
		max_y=paper.height-paper.width/2-1;
		var circ=paper.circle(paper.width/2,get_coord_y(),paper.width/2-1).attr({
			'fill': 'gray',
			'stroke': 'black'
		});
		circ.drag(move, start, end);
	}
	this.resize();	
	this.delete=function(){
		paper.remove()		
	}
}
