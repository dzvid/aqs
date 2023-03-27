function hex_to_ascii(str1)
 {
	var hex  = str1.toString();
	var str = '';
	for (var n = 0; n < hex.length; n += 2) {
		str += String.fromCharCode(parseInt(hex.substr(n, 2), 16));
	}
	return str;
 }

function toHexString(bytes) {
    return bytes.map(function(byte) {
        return ("00" + (byte & 0xFF).toString(16)).slice(-2)
      }).join('');
}

// Decode decodes an array of bytes into an object.
//  - fPort contains the LoRaWAN fPort number
//  - bytes is an array of bytes, e.g. [225, 230, 255, 0]
//  - variables contains the device variables e.g. {"calibration": "3.5"} (both the key / value are of type string)
// The function must return an object, e.g. {"temperature": 22.5}
function Decode(fPort, bytes, variables) {
  
  var tohex = toHexString(bytes);
  var decodedStr = hex_to_ascii(tohex);
  var arrSplit = decodedStr.split(',');
  var arrTrimmed = arrSplit.map(function(value) {
  		return value.trim().replace(/\u0000/g, '')
  	});
  
  if(arrTrimmed[5]) {
    return { 
          "data": {
          "pm2_5": arrTrimmed[0] ? parseInt(arrTrimmed[0]) : 0,
          "pm10": arrTrimmed[1] ? parseInt(arrTrimmed[1]) : 0,
          "temperature": arrTrimmed[2] ? parseFloat(parseFloat(arrTrimmed[2]).toFixed(2)) : 0,
          "humidity": arrTrimmed[3] ? parseFloat(parseFloat(arrTrimmed[3]).toFixed(2)) : 0,
          "pressure": arrTrimmed[4] ? parseFloat(parseFloat(arrTrimmed[4]).toFixed(2)) : 0,
          "dt_collected_at": arrTrimmed[5]+'Z',
          "lat": variables.lat,
          "long": variables.long
    	}
  	};
  } else {
  	return null;
  }
  
}
