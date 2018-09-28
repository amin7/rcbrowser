xmlHttp=createXmlHttpObject();

function createXmlHttpObject(){
    if(window.XMLHttpRequest){
      xmlHttp=new XMLHttpRequest();
    }else{
      xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
  }

var curX,curY;
function cameraSendPosition(x,y){
	var nX=parseInt(x);
	var nY=parseInt(y);
	if(curX==nX && curY==nY){
		return;
	}
    if(xmlHttp.readyState==0||xmlHttp.readyState==4){
	  var obj = new Object();
	  obj.X=nX;
	  obj.Y=nY;
	  curX=nX;
	  curY=nY;
	  var data = JSON.stringify(obj);
	  xmlHttp.open('PUT','camera',true);
	  xmlHttp.setRequestHeader("Content-type", "application/json");
	  xmlHttp.send(data);
	  console.log(data);
    }	
}

function minmax(veriable,min,max){
	if(veriable<min){
		return min;
	}
	if(veriable>max){
		return max;
	}
	return veriable;
}

var jCamera;
var cameraUpdateInterval;
var cameraX,cameraY;

function init_camera(container_){
	console.log('started init_camera');
	
	cameraX=50.0;
	cameraY=50.0;
	cameraSendPosition(cameraX,cameraY);
	jCamera	= new VirtualJoystick({
		container	: container_,
		limitStickTravel: true,
		mouseSupport	: true,
		strokeStyle	: 'orange'
	});

	function cameraJoystickDelta(){	
		var deltaX=parseInt(jCamera.deltaX());
		var deltaY=parseInt(jCamera.deltaY());	
		cameraX=minmax(cameraX+deltaX/20,0,100);
		cameraY=minmax(cameraY+deltaY/20,0,100);
		console.log('camera '+cameraX+":"+cameraY);
		cameraSendPosition(cameraX,cameraY)
	}

	jCamera.addEventListener('startValidation', function(x){		
		if( x > window.innerWidth/2 )	return false;
		return true
	});

	jCamera.addEventListener("start",function(){
		clearInterval(cameraUpdateInterval);
		cameraJoystickDelta();
		cameraUpdateInterval=setInterval(function(){
			cameraJoystickDelta()
		}, 100);
		} );

	jCamera.addEventListener("end",function(){
		clearInterval(cameraUpdateInterval);
		console.log("cameraUpdate clearInterval");
	} );	
}
