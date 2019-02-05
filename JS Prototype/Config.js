class Config {
    GetByteArray() {
        return getByteArray(this, this.ini);
    }
    SetByteArray(byteArray) {
        return setByteArray(this, this.ini, byteArray);
    }
}

function iniGetValue(obj, iniRow, iniIndex) {
    var value;

    switch(iniGetLocation(obj, iniRow, iniIndex)) { 
        case "static":
            break;
        default:
            value = obj[iniRow.Location];
    }
    if(!value && typeof iniRow.Type === "string")
    {
        if(iniRow.Type.split("[")[0] === "formula") {
            value = new Float32Array(parseInt(iniRow.Type.split("[")[1].split("]")[0]) + 1);
            $.each(value, function(index, valuevalue) {
                value[index] = iniRow.DefaultValue;
            })
        } else {
            value = iniRow.DefaultValue;
        }
    }
    
    return value;
}

function iniSetValue(obj, iniRow, iniIndex, value) {
    if(typeof iniRow.Type === "string") {
        switch(iniGetLocation(obj, iniRow, iniIndex)) { 
            case "static":
                break;
            default:
                obj[iniRow.Location] = value;
        }
    } else {
        if(!obj[iniRow.Location])
            obj[iniRow.Location] = new ConfigGui(iniRow.Type);
        else
            obj[iniRow.Location].ini = iniRow.Type;
        return setByteArray(obj[iniRow.Location], iniRow.Type, value);
    }
}

function iniGetMin(obj, iniRow) {
    var min = iniRow.Min;

    if(!min && typeof iniRow.Type === "string") {
         switch(iniRow.Type.split("[")[0]) {
             case "uint8":
             case "uint16":
             case "uint32":
                 min = 0;
                 break;
            case "int8":
                min = -128;
                break;
            case "int16":
                min = -32768;
                break;
            case "int32":
                min = -2147483648;
                break;
            case "float":
            case "formula":
                min = -3.402823e+38;
                break;
         }
    }

    return min;
}

function iniGetMax(obj, iniRow) {
    var max = iniRow.Max;

    if(!max && typeof iniRow.Type === "string") {
        switch(iniRow.Type.split("[")[0]) {
            case "uint8":
            case "uint16":
            case "uint32":
                max = 0;
                break;
            case "int8":
                max = 127;
                break;
            case "int16":
                max = 32767;
                break;
            case "int32":
                max = 2147483647;
                break;
            case "float":
            case "formula":
                max = 3.402823e+38;
                break;
        }
    }

    return max;
}

function iniGetStep(obj, iniRow) {
    var step = iniRow.Step;

    if(!step && typeof iniRow.Type === "string") {
        switch(iniRow.Type.split("[")[0]) {
            case "uint8":
            case "uint16":
            case "uint32":
            case "int8":
            case "int16":
            case "int32":
                step = 1;
                break;
            case "float":
            case "formula":
                step = 0.01;
                break;
        }
    }

    return step;
}

function iniGetLabel(obj, iniRow) {
    return iniRow.Label;
}

function iniGetLocation(obj, iniRow, iniIndex) {
    var location = iniRow.Location;

    if(!location) {
        location = "Location" + iniIndex;
    }

    return location;
}

function getByteArray(obj, ini) {
    var byteArray = new Uint8Array();
    $.each(ini, function(iniIndex, iniRow){
        var value = iniGetValue(obj, iniRow, iniIndex);
        
        if(typeof iniRow.Type === "string")
        {
            switch(iniRow.Type) {
                case "bool":
                case "uint8":
                    byteArray = byteArray.concatArray(new Uint8Array([value]));
                    break;
                case "uint16":
                    byteArray = byteArray.concatArray(new Uint8Array(new Uint16Array([value]).buffer));
                    break;
                case "uint32":
                    byteArray = byteArray.concatArray(new Uint8Array(new Uint32Array([value]).buffer));
                    break;
                case "int8":
                    byteArray = byteArray.concatArray(new Uint8Array(new Int8Array([value]).buffer));
                    break;
                case "int16":
                    byteArray = byteArray.concatArray(new Uint8Array(new Int16Array([value]).buffer));
                    break;
                case "int32":
                    byteArray = byteArray.concatArray(new Uint8Array(new Int32Array([value]).buffer));
                    break;
                case "float":
                    byteArray = byteArray.concatArray(new Uint8Array(new Float32Array([value]).buffer));
                    break;
                case "iniselection":
                    byteArray = byteArray.concatArray(value.Value.GetByteArray());
                    break;
                default:
                    if(iniRow.Type.indexOf("[") > -1) {
                        byteArray = byteArray.concatArray(new Uint8Array(value.buffer));
                    } else {
                        throw "getByteArray Value Invalid";
                    }
                    break;
            }
        } else {
            byteArray = byteArray.concatArray(value.GetByteArray());
        }
    });

    return byteArray;
}

function setByteArray(obj, ini, byteArray) {
    var prevLength = byteArray.length;

    $.each(ini, function(iniIndex, iniRow){        
        if(typeof iniRow.Type === "string")
        {
            switch(iniRow.Type) {
                case "bool":
                case "uint8":
                    iniSetValue(obj, iniRow, iniIndex, byteArray[0]);
                    byteArray = byteArray.subarray(1);
                    break;
                case "uint16":
                    iniSetValue(obj, iniRow, iniIndex, new Uint16Array(byteArray.subarray(0, 2))[0]);
                    byteArray = byteArray.subarray(2);
                    break;
                case "uint32":
                    iniSetValue(obj, iniRow, iniIndex, new Uint32Array(byteArray.subarray(0, 4))[0]);
                    byteArray = byteArray.subarray(4);
                    break;
                case "int8":
                    iniSetValue(obj, iniRow, iniIndex, new Int8Array(byteArray.subarray(0, 1))[0]);
                    byteArray = byteArray.subarray(1);
                    break;
                case "int16":
                    iniSetValue(obj, iniRow, iniIndex, new Int16Array(byteArray.subarray(0, 2))[0]);
                    byteArray = byteArray.subarray(2);
                    break;
                case "int32":
                    iniSetValue(obj, iniRow, iniIndex, new Int32Array(byteArray.subarray(0, 4))[0]);
                    byteArray = byteArray.subarray(4);
                    break;
                case "float":
                    iniSetValue(obj, iniRow, iniIndex, new Float32Array(byteArray.subarray(0, 4))[0]);
                    byteArray = byteArray.subarray(4);
                    break;
                case "iniselection":
                    var selectionVal;
                    $.each(iniRow.Selections, function(selectionIndex, selectionValue) {
                        var selectionId;
                        if(selectionValue.ini[0].Location === "static")
                        {
                            switch(selectionValue.ini[0].Type) {
                                case "uint8":
                                    selectionId = byteArray[0];
                                    break;
                                case "uint16":
                                    selectionId = new Uint16Array(byteArray.subarray(0, 2))[0];
                                    break;
                            }
                        }
                        if(selectionId === selectionValue.ini[0].DefaultValue)
                        {
                            selectionVal = {Index: selectionIndex, Value: new ConfigGui(selectionValue.ini)}
                        }
                    });

                    if(selectionVal)
                    {
                        byteArray = byteArray.subarray(selectionVal.Value.SetByteArray(byteArray));
                        iniSetValue(obj, iniRow, iniIndex, selectionVal);
                    }
                    break;
                default:
                    if(iniRow.Type.indexOf("[") > -1) {
                        var arrayLen = parseInt(iniRow.Type.split("[")[1].split("]")[0]);
                        if(iniRow.Type.split("[").length === 3)
                            arrayLen *= parseInt(iniRow.Type.split("[")[2].split("]")[0]);
                        switch(iniRow.Type.split("[")[0]) {
                            case "bool":
                            case "uint8":
                                iniSetValue(obj, iniRow, iniIndex, byteArray.subarray(0, arrayLen));
                                byteArray = byteArray.subarray(arrayLen);
                                break;
                            case "uint16":
                                iniSetValue(obj, iniRow, iniIndex, new Uint16Array(byteArray.subarray(0, 2 * arrayLen)));
                                byteArray = byteArray.subarray(2 * arrayLen);
                                break;
                            case "uint32":
                                iniSetValue(obj, iniRow, iniIndex, new Uint32Array(byteArray.subarray(0, 4 * arrayLen)));
                                byteArray = byteArray.subarray(4 * arrayLen);
                                break;
                            case "int8":
                                iniSetValue(obj, iniRow, iniIndex, new Int8Array(byteArray.subarray(0, arrayLen)));
                                byteArray = byteArray.subarray(arrayLen);
                                break;
                            case "int16":
                                iniSetValue(obj, iniRow, iniIndex, new Int16Array(byteArray.subarray(0, 2 * arrayLen)));
                                byteArray = byteArray.subarray(2 * arrayLen);
                                break;
                            case "int32":
                                iniSetValue(obj, iniRow, iniIndex, new Int32Array(byteArray.subarray(0, 4 * arrayLen)));
                                byteArray = byteArray.subarray(4 * arrayLen);
                                break;
                            case "formula":
                                arrayLen++;
                            case "float":
                                iniSetValue(obj, iniRow, iniIndex, new Float32Array(byteArray.subarray(0, 4 * arrayLen)));
                                byteArray = byteArray.subarray(4 * arrayLen);
                                break;
                        }
                    } else {
                        throw "setByteArray Type Invalid";
                    }
                    break;
            }
        } else {
            byteArray = byteArray.subarray(iniSetValue(obj, iniRow, iniIndex, byteArray));
        }
    });

    return prevLength - byteArray.length;
}