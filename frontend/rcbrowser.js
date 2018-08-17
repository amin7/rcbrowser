xmlHttp=createXmlHttpObject();

function createXmlHttpObject(){
    if(window.XMLHttpRequest){
      xmlHttp=new XMLHttpRequest();
    }else{
      xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
  }

function init(){
	console.log('started');
	var container_=document.getElementById('container');
	var joystick	= new VirtualJoystick({
		container	: document.getElementById('container'),
		mouseSupport	: true,
	});
	let dX;
	let dY;
	
	container_.addEventListener('mousemove', function(){
		let tX=joystick.deltaX();
		let tY=joystick.deltaY();
		if(tX!=dX || tY!=dY){          
          if(xmlHttp.readyState==0||xmlHttp.readyState==4){
        	  var obj = new Object();
              obj.deltaX=tX;
              obj.deltaY=tY;
              var data = JSON.stringify(obj);
    	      xmlHttp.open('PUT','joystick',true);
    	      xmlHttp.setRequestHeader("Content-type", "application/json");
    	      xmlHttp.send(data);
    	      console.log(data);
          }			
		}
		dX=tX;
		dY=tY;
	});
  }