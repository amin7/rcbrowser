function minmax(veriable,min,max){
	if(veriable<min){
		return min;
	}
	if(veriable>max){
		return max;
	}
	return veriable;
}

function on_chasis_stop(){
	console.log('on_stop');
	set_wheels(0,0);
}

function set_wheels(l,r){
	var xmlHttp = new XMLHttpRequest();
	console.log('set_wheels l:r='+l+':'+r);	
	var obj = new Object();	
	obj.wheel_L0=Number(l);    
	obj.wheel_R0=Number(r);    
    var data = JSON.stringify(obj);
    xmlHttp.open('PUT','wheels',true);
    xmlHttp.setRequestHeader("Content-type", "application/json");
    xmlHttp.send(data);
    console.log(data);
}

function set_chasiscamera(y){
	var xmlHttp = new XMLHttpRequest();
	console.log('set_chasiscamera y='+y);	
	var obj = new Object();	
	obj.Y=Number(y);    
    var data = JSON.stringify(obj);
    xmlHttp.open('PUT','chasiscamera',true);
    xmlHttp.setRequestHeader("Content-type", "application/json");
    xmlHttp.send(data);
    console.log(data);	
}
var radar;
var camera_slider;

function get_chasis_radar(timestamp){
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function(){
    if (xmlHttp.readyState == 4){
      if(xmlHttp.status == 200) {
        var res = JSON.parse(xmlHttp.responseText);        
        if(!radar){//init radar
        	radar=new Radar({id:"ultrasonic",
        		minAngle:res.AngleMin,maxAngle:res.AngleMax,angleStep:res.AngleStep	});
        }
        var last_timestamp=0;
        res.radar.forEach(function(item){
        	if(last_timestamp<item.timestamp){
        		last_timestamp=item.timestamp;
        	}    		
			radar.draw(item.distance,item.angle);
		});        
        setTimeout(function(){get_chasis_radar(last_timestamp);},300);
      }
    }
  };
  xmlHttp.open("PUT", "/chasisradar", true);
  xmlHttp.setRequestHeader("Content-type", "application/json");
  xmlHttp.send((timestamp)?('{"timestamp":'+timestamp+'}'):null);
}

function init_driver(){	
	console.log('started init_driver');
	console.log('location.hostname='+document.location.hostname+' port='+document.location.port  );
	
	var jChasis	= new VirtualJoystick({
		container	: document.getElementById('joystick_layer'),
		limitStickTravel: true,
		mouseSupport	: true,
		strokeStyle	: 'cyan'
	});

	jChasis.addEventListener("move",function(){
		var deltaX=parseInt(jChasis.deltaX());
		var deltaY=-parseInt(jChasis.deltaY());		//
		var l=minmax((deltaY+deltaX),-jChasis._stickRadius,jChasis._stickRadius);
		var r=minmax((deltaY-deltaX),-jChasis._stickRadius,jChasis._stickRadius);
		if(deltaY>0){			
			set_wheels(l,r);
		}else{			
			set_wheels(l,r);
		}
	});

	jChasis.addEventListener("end",function(){		
		on_chasis_stop();
	} );	
		
	camera_slider=new slider({container:"camera_control_"}, 
			function(value){set_chasiscamera(~~value)});
		
	get_chasis_radar();
}

function todgeElement(id) { 
	console.log('todge '+id);
	var x = document.getElementById(id);
    if (x.style.display !== "none") {
    	x.style.display = "none";
    } else {
    	x.style.display = "block";
    }
}

function resize(){
	console.log('resize');
	if(radar)
		radar.resize();
	camera_slider.resize();
}