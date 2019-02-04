Uint8Array.prototype.toHex = function() { // buffer is an ArrayBuffer
    return "0x" + Array.prototype.map.call(new Uint8Array(this), x => ('00' + x.toString(16)).slice(-2)).join(' 0x');
}

Uint8Array.prototype.concatArray = function(b) { // a, b TypedArray of same type
    var c = new (this.constructor)(this.length + b.length);
    c.set(this, 0);
    c.set(b, this.length);
    return c;
}

function getFileContents(url)
{
    var contents

    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, false);

    xhr.onload = function(e) {
        contents = e.target.response;
    };

    xhr.send()
    return contents;
}

var GUID = 0;
function getGUID(){
    return "GUID" + (GUID++);
}