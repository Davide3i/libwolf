var ws;

function property(name, description, id, type, value, secTimestamp, uSecTimestamp){

	this.name = name;
	this.description = description;
	this.id = id;
	this.type = type;
	this.value = value;
	this.secTimestamp = secTimestamp;
	this.uSecTimestamp = uSecTimestamp;
        
        this.toString = function(){
            return " name: "+ this.name +
                   " description: " + this.description +
                   " id: " + this.id +
                   " type: " + this.type +
                   " value: " + this.value +
                   " Timestamp: " + this.secTimestamp +"."+this.uSecTimestamp;
        };
}

function initConnection(){
    
    if (typeof MozWebSocket !== "undefined") {
	ws = new MozWebSocket(get_appropriate_ws_url(),"property-protocol");
    } else {
	ws = new WebSocket(get_appropriate_ws_url(),"property-protocol");
    }

    ws.binaryType = 'arraybuffer';

    try {
        
            ws.onmessage = function got_packet(evt){	
                var message = evt.data;
                var receivedProperty = messageGetUpdateDecoder(message);            
                wolfPropertyCallback(receivedProperty);
            };
            
	}catch(exception){
		alert('<p>Error' + exception);  
	}
}

function get_appropriate_ws_url() {
	var u = document.URL;
	u = u.substr(7);
	u = u.split('/');
	return "ws://" + u[0]; 	//example:  ws://127.0.0.1:7681
}

function getProperty(id){
    messageGetQueryEncoder(id);
}

function messageGetQueryEncoder(id) {
/*
    var message = new ArrayBuffer(5);
    dv = new DataView(message);
        
    dv.setInt8(0,1);
    dv.setInt32(1,id);
        
    message.dv.getString(,,);
        
    return message;
*/
    send("ciao"); //TODO !!!
}

function send(mesg){
	ws.send(mesg);
}

//param message type: ArrayBuffuer
//return type: property
function messageGetUpdateDecoder(message){
	var id, name, description, type, value, secTimestamp, uSecTimestamp;
       
	var offset=1;
        
        //add  a new usefull method to DataView objects for extracting string
        DataView.prototype.getString=function(offset,length){
            var str="";
            
            for(var i=0;i<length;i++){
                var charByte = dv.getUint8(offset);
                str += String.fromCharCode(charByte);
                offset++;
            }
           
            return str;
        };

        dv = new DataView(message);
      
	id = dv.getUint32(offset);
        offset+=4;
        
	var nameLength = dv.getUint8(offset);
        offset++;
       
        name=dv.getString(offset,nameLength);
        offset+=nameLength;
       
	type = dv.getUint8(offset);
	offset++;
	
	switch(type) {

            case 1: value = dv.getUint32(offset);
                    offset += 4;
                    break;

            case 2: value = dv.getFloat32(offset);
		    offset += 4;
                    break;

            case 3: var stringLength = dv.getUint8(offset);
                    offset++;
                    value = dv.getString(offset,stringLength);
                    offset+=stringLength;
                        
            default: break;
	}
       
	var descriptionLength = dv.getUint8(offset);
	offset++;
        
        description=dv.getString(offset,descriptionLength);
        offset+=descriptionLength;
        
	secTimestamp = dv.getUint32(offset);
	offset+= 4;
	uSecTimestamp = dv.getUint32(offset);

	var p = new property(name, description, id, type, value, secTimestamp, uSecTimestamp);

	return p;
}
