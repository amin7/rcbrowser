xmlHttp=createXmlHttpObject();

function createXmlHttpObject(){
    if(window.XMLHttpRequest){
      xmlHttp=new XMLHttpRequest();
    }else{
      xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
  }

;

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
function init_driver(){	
	console.log('started init_driver');
	console.log('location.hostname='+document.location.hostname+' port='+document.location.port  );
	
	var jChasis	= new VirtualJoystick({
		container	: document.getElementById('joystick_layer'),
		limitStickTravel: true,
		mouseSupport	: true,
		strokeStyle	: 'cyan'
	});
	
//	jChasis.addEventListener('startValidation', function(x){		
//		if( x <= window.innerWidth/2 )	return false;
//		return true
//	});

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
	
	radar=new radar("ultrasonic");
	camera_slider=new slider({container:"camera_control_"}, 
			function(value){set_chasiscamera(~~value)});
	
	
	var eagle=-90;
	setInterval(function(){
	eagle+=180/20;
	if(eagle>=90){
		eagle=-90;
	}
	radar.draw(~~(Math.random() * 100),eagle);		
	},100);
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
	radar.resize();
	camera_slider.resize();
}