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
function on_chasis_stop(){
	console.log('on_stop');
	set_wheels(0,0,0,0);
}

function set_wheels(l0,l1,r0,r1){
	console.log('set_wheels');	
	var obj = new Object();
    obj.wheel_L0=Number(l0);
    obj.wheel_L1=Number(l1);
    obj.wheel_R0=Number(r0);
    obj.wheel_R1=Number(r1);
    var data = JSON.stringify(obj);
    xmlHttp.open('PUT','wheels',true);
    xmlHttp.setRequestHeader("Content-type", "application/json");
    xmlHttp.send(data);
    console.log(data);
}


function update_move(){
	console.log('update_move'
			+'steering_wheel='+range_steering_wheel.value
			+' power='+range_power.value
			+' diff L:R='+range_diff_L.value+':'+range_diff_R.value			
	);	
	var PL=range_power.value*range_steering_wheel.value/100
	var PR=range_power.value-PL;
	
	var PL0=PL*range_diff_L.value/100;	
	var PR0=PR*range_diff_R.value/100;
	range_wheel_L0.value=PL0;
	range_wheel_L1.value=PL-PL0;
	range_wheel_R0.value=PR0;
	range_wheel_R1.value=PR-PR0;
	
	range_wheels_xx_on_input();
}
function init_driver(){
	let container_=document.getElementById('camera');
	console.log('started init_driver');
	console.log('location.hostname='+document.location.hostname+' port='+document.location.port  );
	
	init_camera(container_)
	
	var jChasis	= new VirtualJoystick({
		container	: container_,
		limitStickTravel: true,
		mouseSupport	: true,
		strokeStyle	: 'cyan'
	});
	
	jChasis.addEventListener('startValidation', function(x){		
		if( x <= window.innerWidth/2 )	return false;
		return true
	});

	jChasis.addEventListener("move",function(){
		var deltaX=parseInt(jChasis.deltaX());
		var deltaY=-parseInt(jChasis.deltaY());
		//console.log("move "+deltaX+":"+deltaY );
		var l=minmax((deltaY+deltaX),-100,100);
		var r=minmax((deltaY-deltaX),-100,100);
		if(deltaY>0)
			console.log("power "+l+ ":" +r);
		else
			console.log("power "+r+ ":" +l);
	});

	jChasis.addEventListener("end",function(){		
		on_chasis_stop();
	} );	
	
	//radar("ultrasonic");
  }
