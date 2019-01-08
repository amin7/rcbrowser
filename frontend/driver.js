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

var camera_slider;
function set_get_chasiscamera(y){
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function(){
	    if (xmlHttp.readyState == 4){
	      if(xmlHttp.status == 200) {
	        var res = JSON.parse(xmlHttp.responseText);        
	        if(!camera_slider){//init slider
	        	camera_slider=new slider({container:"camera_control_",value:res.Y}, 
	        			function(value){set_get_chasiscamera(~~value)});	        	
	        }
	      }
	    }
	  };
	var obj = new Object();	
	if(y){
		obj.Y=Number(y);
	}
    var data = JSON.stringify(obj);
    xmlHttp.open('PUT','chasiscamera',true);
    xmlHttp.setRequestHeader("Content-type", "application/json");
    xmlHttp.send(data);
    console.log(data);	
}

function todgeCamera(){
	if(camera_slider){
		showCamera(false);
	}else{
		showCamera(true);
	}
}
function showCamera(show){
	showElement("camera_control_",show);
	if(show){				
		set_get_chasiscamera();
	}else{		
		if(camera_slider){
			camera_slider.delete();
			camera_slider=undefined;			
		}
	}	
}

var radar;
var radar_Interval;
var radar_last_timestamp=0;
function get_chasis_radar(){
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function(){
    if (xmlHttp.readyState == 4){
      if(xmlHttp.status == 200) {
        var res = JSON.parse(xmlHttp.responseText);        
        if(!radar){//init radar
        	radar=new Radar({id:"ultrasonic",
        		minAngle:res.AngleMin,
        		maxAngle:res.AngleMax,
        		angleStep:res.AngleStep,
        		maxDistance:res.MaxDistance});
        }        
        res.radar.forEach(function(item){
        	if(radar_last_timestamp<item.time){
        		radar_last_timestamp=item.time;
        	}    		
			radar.draw(item.dist,item.angl);
		});
      }
    }
  };
  xmlHttp.open("PUT", "/chasisradar", true);
  xmlHttp.setRequestHeader("Content-type", "application/json");  
  if(radar){
	  xmlHttp.send('{"timestamp":'+radar_last_timestamp+'}');
  }else{
	radar_last_timestamp=0;
	xmlHttp.send(null);
  }
}

function todgeRadar(){
	if(radar){
		showRadar(false);
	}else{
		showRadar(true);
	}
}
function showRadar(show){
	showElement("ultrasonic",show);
	if(show){		
		radar_Interval=setInterval(get_chasis_radar,300);
		get_chasis_radar();
	}else{
		clearInterval(radar_Interval);
		if(radar){
			radar.delete();
			radar=undefined;			
		}
	}	
}

function get_status(){
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function(){
    if (xmlHttp.readyState == 4){
      if(xmlHttp.status == 200) {
        var res = JSON.parse(xmlHttp.responseText);        
        console.log(res);
        document.getElementById("vbat").innerHTML="vbat="+res.vbat+"V";
      }
    }
  };
  xmlHttp.open("PUT", "/status", true);
  xmlHttp.setRequestHeader("Content-type", "application/json");  
  xmlHttp.send(null);
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
			
	showCamera(true);
	showRadar(true);
	setInterval(get_status,1000);
}

function showElement(id,show) { 
	
	var x = document.getElementById(id);
    if (!show) {
    	x.style.display = "none";
    } else {
    	x.style.display = "block";
    }
}

function resize(){
	console.log('resize');
	if(radar)
		radar.resize();
	if(camera_slider)
		camera_slider.resize();
}