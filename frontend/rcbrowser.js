xmlHttp=createXmlHttpObject();
var range_rotation;
var range_steering_wheel;
var range_wheel_L0;
var range_wheel_L1;
var	range_wheel_R0;
var range_wheel_R1;
var range_diff_L;
var range_diff_R;
var range_power;

function createXmlHttpObject(){
    if(window.XMLHttpRequest){
      xmlHttp=new XMLHttpRequest();
    }else{
      xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
  }

function update_rotate(update_rotate){
	console.log('update_rotate='+update_rotate);
}

function on_stop(){
	console.log('on_stop');
	range_rotation.value=range_rotation.defaultValue;
	range_steering_wheel.value=range_steering_wheel.defaultValue;
	range_wheel_L0.value=range_wheel_L0.defaultValue;
	range_wheel_L1.value=range_wheel_L1.defaultValue;
	range_wheel_R0.value=range_wheel_R0.defaultValue;
	range_wheel_R1.value=range_wheel_R1.defaultValue;
	range_diff_L.value=range_diff_L.defaultValue;
	range_diff_R.value=range_diff_R.defaultValue;
	range_power.value=range_power.defaultValue;
	range_wheels_xx_on_input();
}


function update_rotation(){
	console.log('update_rotation='+range_rotation.value);
	range_wheel_L1.value=range_wheel_L0.value=range_rotation.value;	
	range_wheel_R1.value=range_wheel_R0.value=range_rotation.value*(-1);
	range_wheels_xx_on_input();
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

function range_wheels_xx_on_input(){
	set_wheels(range_wheel_L0.value,range_wheel_L1.value,
			range_wheel_R0.value,range_wheel_R1.value);
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

var cameraX;
var cameraY;

function cameraSendPosition(x,y){
    if(xmlHttp.readyState==0||xmlHttp.readyState==4){
	  var obj = new Object();
	  obj.X=Number(x);
	  obj.Y=Number(y);
	  var data = JSON.stringify(obj);
	  xmlHttp.open('PUT','camera',true);
	  xmlHttp.setRequestHeader("Content-type", "application/json");
	  xmlHttp.send(data);
	  console.log(data);
    }	
}

var joystick;
var cameraUpdateInterval;

function cameraJoystickDelta(){	
	var deltaX=parseInt(joystick.deltaX());
	var deltaY=parseInt(joystick.deltaY());
	cameraX.value=parseFloat(cameraX.value)+deltaX/20;
	cameraY.value=parseFloat(cameraY.value)-deltaY/20;
	console.log('camera '+cameraX.value+":"+cameraY.value);
}
function init(){
	console.log('started');
	console.log('location.hostname='+document.location.hostname+' port='+document.location.port  );
	range_rotation=document.getElementById('rotation');
	range_wheel_L0=document.getElementById('wheel_L0');
	range_wheel_L1=document.getElementById('wheel_L1');
	range_wheel_R0=document.getElementById('wheel_R0');
	range_wheel_R1=document.getElementById('wheel_R1');
	range_steering_wheel=document.getElementById('steering_wheel');
	range_diff_L=document.getElementById('diff_L');
	range_diff_R=document.getElementById('diff_R');
	range_power=document.getElementById('power');	
		
	range_rotation.addEventListener("input", update_rotation);
	range_rotation.addEventListener("mouseup", function(){
		range_rotation.value=0;
		update_rotation();}
	);	
	
	range_steering_wheel.addEventListener("input", update_move);
	range_diff_L.addEventListener("input", update_move);
	range_diff_R.addEventListener("input", update_move);
	range_power.addEventListener("input", update_move);
	range_wheel_L0.addEventListener("input", range_wheels_xx_on_input);
	range_wheel_L1.addEventListener("input", range_wheels_xx_on_input);
	range_wheel_R0.addEventListener("input", range_wheels_xx_on_input);
	range_wheel_R1.addEventListener("input", range_wheels_xx_on_input);
	
	cameraY=document.getElementById('cameraY');
	cameraX=document.getElementById('cameraX');

	 joystick	= new VirtualJoystick({
		container	: document.getElementById('fpv'),
		limitStickTravel: true,
		mouseSupport	: true
	});
	joystick.addEventListener('startValidation', function(x){
		console.log('validataion '+x);
		//var touch	= event.changedTouches[0];
		if( x < 100 )	return false;
		return true
	});

	let container_=document.getElementById('fpv');
		joystick.addEventListener("start",function(){
			clearInterval(cameraUpdateInterval);
			cameraJoystickDelta();
			cameraUpdateInterval=setInterval(function(){
				cameraJoystickDelta()
			}, 100);
			} );
	
		joystick.addEventListener("end",function(){
			clearInterval(cameraUpdateInterval);
			console.log("cameraUpdate clearInterval");
		} );
	
	cameraX.addEventListener("input",function(){cameraSendPosition(cameraX.value,cameraY.value);})
	cameraY.addEventListener("input",function(){cameraSendPosition(cameraX.value,cameraY.value);})
	radar("ultrasonic");
  }
