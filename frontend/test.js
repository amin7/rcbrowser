xmlHttp=createXmlHttpObject();

function createXmlHttpObject(){
    if(window.XMLHttpRequest){
      xmlHttp=new XMLHttpRequest();
    }else{
      xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
  };
  
  var pwm_pin;
  var pwm_value;
  var pwm_range;
  function pwm_set_range(value){
	  pwm_value.value=value;
	  pwm_set(pwm_pin.value,value)
  }
  function pwm_set_value(value){
	  pwm_range.value=value;
	  pwm_set(pwm_pin.value,value)
  }
  function pwm_min(value){
	  pwm_value.min=value;
	  pwm_range.min=value;
  }
  function pwm_max(value){
	  pwm_value.max=value;
	  pwm_range.max=value;
  }
  function pwm_set(pin,value){
	var obj = new Object();	
	obj.pwm=Number(pin);    
	obj.value=Number(value);    
    var data = JSON.stringify(obj);
    xmlHttp.open('PUT','test',true);
    xmlHttp.setRequestHeader("Content-type", "application/json");
    xmlHttp.send(data);
    console.log(data);
  }
  
 function init(){	 
	console.log('started');	 

	pwm_pin=document.getElementById('pwm_pin');
	pwm_value=document.getElementById('pwm_value');
	pwm_range=document.getElementById('pwm_range');
}