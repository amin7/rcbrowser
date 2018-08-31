//http://dmitrybaranovskiy.github.io/raphael/
function radar(id) {
	console.log('radar');
	var paper = Raphael(id);
	var radius=paper.height;
	if(radius>paper.width/2){
		radius=paper.width/2;
	}

	paper.rect(0,0,paper.width,paper.height);
	paper.path([		
		//arcs...
				"M",0,paper.height,
				"A",radius,radius,0,0,1,paper.width,paper.height,
				"M",radius/2,paper.height,
				"A",radius/2,radius/2,0,0,1,paper.width-radius/2,paper.height,
				"M",paper.width/2-5,paper.height,
				"A",5,5,0,0,1,paper.width/2+5,paper.height,
				]);
	
	
   return "Hello";
}