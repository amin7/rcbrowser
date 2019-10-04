function set_bse_(base,shoulder,elbow){
    var xmlHttp = new XMLHttpRequest();
    var obj = new Object();
    obj.bse=bse=new Object();
    bse.base=Number(base);
    bse.shoulder=180-Number(shoulder);
    bse.elbow=Number(elbow)+Number(shoulder);
    
    var data = JSON.stringify(obj);
    xmlHttp.open('PUT','manipulator',true);
    xmlHttp.setRequestHeader("Content-type", "application/json");
    xmlHttp.send(data);
    console.log(data);
}

function set_bse(){
    set_bse_(document.getElementById('id_base').value,document.getElementById('id_shoulder').value,document.getElementById('id_elbow').value);
}
  
var BackArm_mm = 80;
var ForeArm_mm = 80;
var d_Min = 15;
var d_Max = 155;
var R_offset_mm = 7.5;

function InverseKinematicsTransform(R,H) {
    var sqr_d;
    var d;
    var Shoulder_Rad_1;
    var Shoulder_Rad_2;

    sqr_d = R * R + H * H;
    d = Math.sqrt(sqr_d);
    //Serial.println (sqr_d);
    if (d > BackArm_mm + ForeArm_mm || d > d_Max) // trivially out of range
      return 1;
    if (d <= 0 || d < d_Min) // not possible for structure of arm
      return 2;

    R = R - R_offset_mm;
    Elbow = Math.acos((BackArm_mm * BackArm_mm + ForeArm_mm * ForeArm_mm - sqr_d) / (2 * BackArm_mm * ForeArm_mm));

    Shoulder_Rad_1 = Math.asin((ForeArm_mm * Math.sin(Elbow)) / d);
    Shoulder_Rad_2 = Math.atan(H / R);

    //Serial.println (Shoulder_Rad_1);
    //Serial.println (Shoulder_Rad_2);

    Shoulder = (Shoulder_Rad_1 + Shoulder_Rad_2) * 180 / Math.PI;
    Elbow = Elbow * 180 / Math.PI;
    var obj = {
            "shoulder":  Shoulder,
            "elbow": Elbow
          };
    return obj;
  }

function set_xyz_trans(){
    var x=document.getElementById('id_x').value;
    var y=document.getElementById('id_y').value;
    var z=document.getElementById('id_z').value;
    var res=new InverseKinematicsTransform(x,z);
    console.log(res);
    if(res.shoulder)
        document.getElementById('id_shoulder').value=res.shoulder;
    if(res.elbow)
        document.getElementById('id_elbow').value=res.elbow;
    set_bse();
}
 function init(){	 
	console.log('started');	 
}
