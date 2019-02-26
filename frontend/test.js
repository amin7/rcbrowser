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
  
  var chIndex=0;
  
  var accelChart;
  var accel_data;
  
  var poss_chart;
  var poss_data;
  
  var speed_chart;
  var speed_data;
  
  function prepareChartsData(data){
      data.addColumn('number', "index");
      data.addColumn('number', "x");
      data.addColumn('number', "y");
      data.addColumn('number', "z");
  }
  
  function drawCharts(){
      accelChart = new google.visualization.LineChart(document.getElementById("accel_id"));
      accel_data = new google.visualization.DataTable();
      prepareChartsData(accel_data);
      
      poss_chart = new google.visualization.LineChart(document.getElementById("poss_id"));
      poss_data = new google.visualization.DataTable();
      prepareChartsData(poss_data);
      
      speed_chart = new google.visualization.LineChart(document.getElementById("speed_id"));
      speed_data = new google.visualization.DataTable();
      prepareChartsData(speed_data);      
  }
  var orientation;
  
  function get_orientation(){
		var xmlHttp = new XMLHttpRequest();
		xmlHttp.onreadystatechange = function(){
	    if (xmlHttp.readyState == 4){
	      if(xmlHttp.status == 200) {
	        var res = JSON.parse(xmlHttp.responseText);        
	        console.log(res);
	        //orientation.set(res.roll,res.pitch,res.yaw);
	        orientation.setq(res.quaternion.y,res.quaternion.z,res.quaternion.x,res.quaternion.w);
	        document.getElementById("id_qx").innerHTML="qz="+parseInt(res.ypr.yaw/Math.PI*360);
	        if(accel_data){
	            chIndex++;	        
	            accel_data.addRow([chIndex, res.accel.x, res.accel.y, res.accel.z]);
	            accelChart.draw(accel_data);
	            
	            //speed_data.addRow([chIndex, res.speed.x, res.speed.y, res.speed.z]);
	            speed_data.addRow([chIndex, null, res.speed.y, null]);
	            speed_chart.draw(speed_data);
	            
//	            poss_data.addRow([chIndex, res.position.x, res.position.y, res.position.z]);
	            poss_data.addRow([chIndex,null, res.position.y, null]);
	            poss_chart.draw(poss_data);
	        }
	      }
	    }
	  };
	  xmlHttp.open("PUT", "/mpu6050", true);
	  xmlHttp.setRequestHeader("Content-type", "application/json");  
	  xmlHttp.send(null);
	}
  
 function init(){	 
	console.log('started');	 

	pwm_pin=document.getElementById('pwm_pin');
	pwm_value=document.getElementById('pwm_value');
	pwm_range=document.getElementById('pwm_range');
	
	google.charts.load('current', {'packages':['line','corechart']});
	google.charts.setOnLoadCallback(drawCharts);
	  
	orientation=new Orientation({id:"orient_id"});
	setInterval(get_orientation,100);
}
