xmlHttp=createXmlHttpObject();

function createXmlHttpObject(){
    if(window.XMLHttpRequest){
      xmlHttp=new XMLHttpRequest();
    }else{
      xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
  };
  
var manipulatorX;
var manipulatorY;
var manipulatorZ;
function update_move(){		
	var obj = new Object();
	obj.cmd='manipulator';
	obj.X=Number(manipulatorX.value);
	obj.Y=Number(manipulatorY.value);
	obj.Z=Number(manipulatorZ.value);    
    var data = JSON.stringify(obj);
    xmlHttp.open('PUT','command',true);
    xmlHttp.setRequestHeader("Content-type", "application/json");
    xmlHttp.send(data);
    console.log(data);	
}
 function init(){
	console.log('started');  	
	manipulatorX=document.getElementById('manipulatorX');
	manipulatorY=document.getElementById('manipulatorY');
	manipulatorZ=document.getElementById('manipulatorZ');
	
	manipulatorX.addEventListener("input", update_move);
	manipulatorY.addEventListener("input", update_move);
	manipulatorZ.addEventListener("input", update_move);
}