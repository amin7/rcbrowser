//https://github.com/mrdoob/three.js/blob/master/examples/webgl_geometry_cube.html
//https://threejs.org/
//https://threejs.org/editor/
function Orientation(opts){
	opts			= opts			|| {};	
	console.log('Orientation init opts='+opts);
	const place_id_ = document.getElementById(opts.id);
	
	var yaw_,pitch_,roll_;
	var camera, scene, renderer;
	var mesh;
	
	function to2D(x,y,z){		
		return {x:x,y:y};
	}
	init();
	//animate();
	function init() {
		
		scene = new THREE.Scene();
		renderer = new THREE.WebGLRenderer( { alpha: true  } );
		renderer.setPixelRatio( window.devicePixelRatio );
		renderer.setSize( place_id_.offsetWidth, place_id_.offsetHeight );
		place_id_.appendChild( renderer.domElement );
		
		camera = new THREE.PerspectiveCamera( 70, place_id_.offsetWidth / place_id_.offsetHeight, 1, 1000 );
		camera.position.z = 20;
		
		var objectLoader = new THREE.ObjectLoader();
		objectLoader.load( "../gfx/model_orientation.json", function ( obj ) {
			mesh = obj;
		 	scene.add( obj );
		} );
		//var texture = new THREE.TextureLoader().load( 'textures/crate.gif' );
//		var geometry = new THREE.TetrahedronBufferGeometry( 200);
//		var material = new THREE.MeshBasicMaterial({wireframe:true} );
//		mesh = new THREE.Mesh( geometry,material );
//		scene.add( mesh );
//		
		//
		//window.addEventListener( 'resize', onWindowResize, false );
	}
	function animate() {
		requestAnimationFrame( animate );
		mesh.rotation.x += 0.005;
		mesh.rotation.y += 0.01;
		renderer.render( scene, camera );
	}
	this.set=function(roll,pitch,yaw){
		mesh.rotation.z = roll*Math.PI/180;
		mesh.rotation.x = pitch*Math.PI/180;
		mesh.rotation.y = yaw*Math.PI/180;
		renderer.render( scene, camera );		
	}
}