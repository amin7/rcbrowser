//http://dmitrybaranovskiy.github.io/raphael/
function Radar(opts){
	opts			= opts			|| {};	
	console.log('radar init opts='+opts);
	const radar_id_ = document.getElementById(opts.id);
	const minAngle = opts.minAngle||-90;
	const maxAngle = opts.maxAngle||90;	
	const angleStep= opts.angleStep||15;
	const maxDistance = opts.maxDistance||100;
	const showRangeArc = opts.showRangeArc||10;
	const k_elipse=0.5;
	var showedDistance=maxDistance;
	var range_arc;
	var sonic =[];
	var cash_val=[];	
	var paper = Raphael(radar_id_);
	var centerP; // Center points
	var radius;
	
	function get_range_arc(){
		let range_arc="";
		let radius_arc=0;
		if(showRangeArc)
		while(1){
			radius_arc+=showRangeArc;
			if(radius_arc>showedDistance){
				break;
			}
			let length=Math.round(radius_arc/showedDistance*radius);
			range_arc+=["M",centerP.x-length,centerP.y];
			range_arc+=["A",length,Math.round(length * k_elipse),0,0,1,centerP.x+length,centerP.y];
		}
		range_arc+=["M",centerP.x-2,centerP.y];
		range_arc+=["A",2,2,0,0,1,centerP.x+2,centerP.y];
		return range_arc;
	}
	
	this.resize=function(){
		paper.clear();
		sonic =[];		
		paper.setSize(radar_id_.offsetWidth ,radar_id_.offsetHeight );
		centerP = {x:paper.width/2, y:paper.height}; // Center points
		radius=paper.height;
		if(radius>paper.width/2){
			radius=paper.width/2;
		}
		range_arc=paper.path(get_range_arc()).attr({
			'stroke': 'black',
			"stroke-width": 1
		});		
		}
	this.resize();	
		
	function getIndex(andle){
		if(andle<minAngle||andle>maxAngle){
			return -1;//out of range
		}		
		return ~~((andle-minAngle)/angleStep)
	}
	function getSector(Distance, andle){
		let length=Distance/showedDistance*radius;
		if(length>radius){
			length=radius;
		}
			
		let andleRad=andle*Math.PI/180;
		let sin_angle=Math.sin(andleRad);
		let cos_angle=Math.cos(andleRad);
		let drawSector;		
		drawSector=["M",centerP.x+sin_angle * radius,centerP.y-cos_angle*radius*k_elipse];
		drawSector+=["L",centerP.x+sin_angle * length,centerP.y-cos_angle*length*k_elipse];
		andleRad+=angleStep*Math.PI/180;
		let sin_angle_step=Math.sin(andleRad);
		let cos_angle_step=Math.cos(andleRad);
		drawSector+=["L",centerP.x+sin_angle_step * length,centerP.y-cos_angle_step*length*k_elipse];
		drawSector+=["L",centerP.x+sin_angle_step * radius,centerP.y-cos_angle_step*radius*k_elipse];
		drawSector+=["Z"];
		return drawSector;
	}
	this.draw=function(distance, andle){		
		if(distance>maxDistance){
			distance=maxDistance;
		}		
		let index=getIndex(andle);
		if(index==-1){
			console.log('out of range');
			return;
		}
		cash_val[index]=distance;
		let drawPath=getSector(distance,minAngle+angleStep*index)		
	
		if(sonic[index]){
			sonic[index].animate({path: drawPath},200, function() {
				this.attr({path: drawPath});
			});		
		}else{
			sonic[index]=paper.path(drawPath).attr({		
				"fill": "225-#fbb03b:20-#fbb03b:50",				
				'stroke': 'black',
				"stroke-width": 1
			});
		}	
	}
	this.setShowDistance=function(distance){
		showedDistance=distance;
		if(showedDistance>maxDistance){
			showedDistance=maxDistance;
		}	
		cash_val.forEach(function(distance,index){
			let drawPath=getSector(distance,minAngle+angleStep*index);			
			sonic[index].animate({path: drawPath},100, function() {
				this.attr({path: drawPath});
			});
		});
		
		range_arc_new=get_range_arc();
		range_arc.animate({path: range_arc_new},100, function() {
			this.attr({path: range_arc_new});
		});		
		}
	this.delete=function(){
		paper.remove();
	}
	this.getMaxDistance=function(){return maxDistance;}
}