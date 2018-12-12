//http://dmitrybaranovskiy.github.io/raphael/
function radar(id){
	console.log('radar');	
	const minAngle = -90;
	const maxAngle = 90;
	const sectors=20;
	const angleStep=(maxAngle-minAngle)/sectors;
	const maxDistance = 100;		
	var sonic =[];	
	var paper = Raphael(id);
	var centerP = {x:paper.width/2, y:paper.height}; // Center points
		
	var radius=paper.height;
	if(radius>paper.width/2){
		radius=paper.width/2;
	}
	function getIndex(andle){
		if(andle<minAngle||andle>maxAngle){
			return -1;//out of range
		}		
		return ~~((andle-minAngle)/angleStep)
	}
	function getSector(Distance, andle){
		let length=Distance/maxDistance*radius;
		let andleRad=andle*Math.PI/180;
		let sin_angle=Math.sin(andleRad);
		let cos_angle=Math.cos(andleRad);
		let drawSector;
		drawSector=["M",centerP.x+sin_angle * radius,centerP.y-cos_angle*radius];
		drawSector+=["L",centerP.x+sin_angle * length,centerP.y-cos_angle*length];
		andleRad+=angleStep*Math.PI/180;
		let sin_angle_step=Math.sin(andleRad);
		let cos_angle_step=Math.cos(andleRad);
		drawSector+=["L",centerP.x+sin_angle_step * length,centerP.y-cos_angle_step*length];
		drawSector+=["L",centerP.x+sin_angle_step * radius,centerP.y-cos_angle_step*radius];
		drawSector+=["Z"];
		return drawSector;
	}
	this.draw=function(distance, andle){
		console.log('draw radar ['+andle+']='+distance);			
		if(distance>maxDistance){
			distance=maxDistance;
		}		
		let index=getIndex(andle);
		if(index==-1){
			console.log('out of range');
			return;
		}
				
		let drawPath=getSector(distance,minAngle+angleStep*index)		
	
		if(sonic[index]){
			sonic[index].animate({path: drawPath},200, function() {
				this.attr({path: drawPath});
			});		
		}else
		{
			sonic[index]=paper.path(drawPath).attr({		
				"fill": "225-#fbb03b:20-#fbb03b:50",				
				'stroke': 'black',
				"stroke-width": 1
			});
		}
	}		
}