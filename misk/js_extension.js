//https://github.com/LLK/scratchx/wiki

new (function() {
    var ext = this;  
    var is_connected=false
    var server_adress="192.168.1.223";
    var server_port=8000;    

    function get_server_cmd_int(){
      return "http://"+server_adress+":"+server_port;
    }
    // Cleanup function when the extension is unloaded
    ext._shutdown = function() {};

   ext._getStatus = function() {      
      return {status: 2, msg: 'Ready'};
    };

    ext.test_connection = function() {
           return is_connected;
        }
    ext.get_config = function(callback) {
                 $.ajax({              
              url: get_server_cmd_int()+"/config",
              dataType: 'jsonp',
              success: function( resp ) {                  
                is_connected=true;
                callback();
              },
              error: function(){                
                is_connected=false;
                callback();
              },
              timeout: 1000
            });                   
    };

 
    ext.set_server = function(adress, port,callback) {
      server_adress=adress;
      server_port=port;
      ext.get_config(function(){        
          callback(is_connected?"connected":"error");
      })
      
      //test ip +addres and set ok      
    };

    ext.rotate_left = function(degree,callback) {
      if(false==is_connected){
        callback();
        return;
      }
     $.ajax({              
      url: server_adress+":"+server_port+"/config",
      dataType: 'jsonp',
      success: function( resp ) {                  
        callback();
      },
      error: function(){                                
        callback();
      },
      timeout: 1000
    });                   
    };

    ext.rotate_right = function(degree,callback) {
      if(false==is_connected){
        callback();
        return;
      }
     $.ajax({              
      url: server_adress+":"+server_port+"/config",
      dataType: 'jsonp',
      success: function( resp ) {                  
        callback();
      },
      error: function(){                                
        callback();
      },
      timeout: 1000
    });                   
    };

    ext.move = function(speed,callback) {
      if(false==is_connected){
        callback();
        return;
      }
     $.ajax({              
      url: server_adress+":"+server_port+"/config",
      dataType: 'jsonp',
      success: function( resp ) {                  
        callback();
      },
      error: function(){                                
        callback();
      },
      timeout: 1000
    });                   
    };
    ext.motor = function(left,right,callback) {
      if(false==is_connected){
        callback();
        return;
      }
     $.ajax({              
      url: server_adress+":"+server_port+"/config",
      dataType: 'jsonp',
      success: function( resp ) {                  
        callback();
      },
      error: function(){                                
        callback();
      },
      timeout: 1000
    });                   
    };

    // Block and block menu descriptions
    var descriptor = {
        blocks: [
            // ['', 'run alarm after %n seconds', 'set_alarm', '2'],
            // ['h', 'when alarm goes off', 'when_alarm'],
            ['R', 'server %s %n', 'set_server', server_adress, server_port],
            ['b', 'is_connected', 'test_connection'],
            ['w', 'get_config', 'get_config'],
            ['w', 'rotate << %n', 'rotate_left',90],
            ['w', 'rotate %n >>', 'rotate_right',90],
            ['w', 'move %n', 'move',100],
            ['w', 'stop', 'move',0],
            ['w', 'motor %n - %n', 'motor',100,100],
            
        ]
    };

    // Register the extension
    ScratchExtensions.register('Alarm extension', descriptor, ext);
})();