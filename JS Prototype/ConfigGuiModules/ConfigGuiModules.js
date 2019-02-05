var numberConfigGuiTemplate;
function getNumberConfigGui(id, label, value, min, max, step, callBack) {
    if(!numberConfigGuiTemplate)
        numberConfigGuiTemplate = getFileContents("ConfigGuiModules/Number.html");
    var template = numberConfigGuiTemplate;
    template = template.replace(/[$]id[$]/g, id);
    template = template.replace(/[$]label[$]/g, label);
    template = template.replace(/[$]value[$]/g, value);
    template = template.replace(/[$]min[$]/g, min);
    template = template.replace(/[$]max[$]/g, max);
    template = template.replace(/[$]step[$]/g, step);
    if(callBack) {
        $(document).on("change", "#" + id, function(){
            callBack($(this).val());
        });
    }
    return template;
}

function getFormulaConfigGui(id, label, values, min, max, step, callBack) {
    var template = "<label>" + label + ":</label>";
    for(var i = values.length-1; i > 0; i--)
    {
        template += "<input id=\"" + id + i + "\" type=\"number\" min=\"" + min + "\" max=\"" + max + "\" step=\"" + step + "\" value=\"" + values[i] + "\"/>";
        if(i > 1)
            template += " x<sup>" + i + "</sup> + ";
        else
            template += " x + ";
    }
    template += "<input id=\"" + id + 0 + "\" type=\"number\" min=\"" + min + "\" max=\"" + max + "\" step=\"" + step + "\" value=\"" + values[0] + "\"/>";
    
    if(callBack) {
        $.each(values, function(index, value) {
            $(document).on("change", "#" + id + index, function(){
                callBack(index, $(this).val());
            });
        });
    }

    return template;
}

var checkBoxConfigGuiTemplate;
function getCheckBoxConfigGui(id, label, value, callBack) {
    if(!checkBoxConfigGuiTemplate)
        checkBoxConfigGuiTemplate = getFileContents("ConfigGuiModules/CheckBox.html");
    var template = checkBoxConfigGuiTemplate;
    template = template.replace(/[$]id[$]/g, id);
    template = template.replace(/[$]label[$]/g, label);
    if(value)
        template = template.replace(/[$]checked[$]/g, "checked");
    else
        template = template.replace(/[$]checked[$]/g, "");

    if(callBack) {
        $(document).on("change", "#" + id, function(){
            callBack(this.checked);
        });
    }

    return template;
}

var selectionConfigGuiTemplate;
function getSelectionConfigGui(id, label, value, selections, callBack) {
    if(!selectionConfigGuiTemplate)
        selectionConfigGuiTemplate = getFileContents("ConfigGuiModules/Selection.html");
    var template = selectionConfigGuiTemplate;
    template = template.replace(/[$]id[$]/g, id);
    template = template.replace(/[$]label[$]/g, label);
    var selectionHtml = "";
    $.each(selections, function(selectionIndex, selectionValue) {
        if(selectionIndex === value.Index)
            selectionHtml += "<option selected value=\"" + selectionIndex + "\">" + selectionValue.Name + "</option>";
        else
            selectionHtml += "<option value=\"" + selectionIndex + "\">" + selectionValue.Name + "</option>";
    });
    template = template.replace(/[$]selections[$]/g, selectionHtml);
    
    if(callBack) {
        $(document).on("change", "#" + id, function(){
            callBack($(this).val());
        });
    }

    return template;
}

function getIniConfigGui(obj, ini, idPrefix, callBack) {
    var template = "";

    $.each(ini, function(iniIndex, iniRow){    
        var location = iniGetLocation(obj, iniRow, iniIndex);
        
        if(location === "static")
            return;

        template += "<span id=\"span" + idPrefix + location + "\">"

        var label = iniGetLabel(obj, iniRow);
        var value = iniGetValue(obj, iniRow, iniIndex);
        var min = iniGetMin(obj, iniRow);
        var max = iniGetMax(obj, iniRow);
        var step = iniGetStep(obj, iniRow);

        if(typeof iniRow.Type === "string")
        {
            switch(iniRow.Type.split("[")[0]) {
                case "bool":
                    template += getCheckBoxConfigGui(idPrefix + location, label, value, function(value){
                        obj[location] = value;
                        if(callBack)
                            callBack(obj);
                    });
                    break;
                case "uint8":
                case "uint16":
                case "uint32":
                case "int8":
                case "int16":
                case "int32":
                case "float":
                    template += getNumberConfigGui(idPrefix + location, label, value, min, max, step, function(value){
                        obj[location] = value;
                        if(callBack)
                            callBack(obj);
                    });
                    break;
                case "formula":
                    template += getFormulaConfigGui(idPrefix + location, label, value, min, max, step, function(index, value) {
                        obj[location][index] = value;
                        if(callBack)
                            callBack(obj);
                    });
                    break;
                case "iniselection":
                    template += "<span>" + getSelectionConfigGui(idPrefix + location, label, value, iniRow.Selections, function(value) {
                        obj[location].Index = value;
                        debugger;
                        if(!obj[location].Value)
                            obj[location] = { Index: value.Index, Value: new ConfigGui(iniRow.Selections[value].ini)}
                        else
                            obj[location].Value.ini = iniRow.Selections[value].ini
                        $("#span" + idPrefix + location + "selection").html(obj[location].Value.GetHtml());
                        if(callBack)
                            callBack(obj);
                    }) + "</span>";
                    if(!value.Value)
                        obj[location] = { Index: value.Index, Value: new ConfigGui(iniRow.Selections[value.Index].ini)}
                    var innerValue;
                    if(iniRow.WrapInConfigContainer)
                        template += "<br>" + wrapInConfigContainerGui("span" + idPrefix + location + "selection", obj[location].Value.GetHtml());
                    else
                        template += "<span id=\"span" + idPrefix + location + "selection\">" + obj[location].Value.GetHtml()+ "</span>";
                    break;
                default:
                    break;
            }
        } else {
            if(!obj[location])
                obj[location] = new ConfigGui(iniRow.Type);

            if(iniRow.WrapInConfigContainer)
                template += wrapInConfigContainerGui(obj[location].GUID, obj[location].GetHtml());
            else
                template += obj[location].GetHtml();
        }

        template += "</span>";
    });

    return template;
}

class ConfigGui extends Config {
    constructor(ini, callBack){
        super();
        this.GUID = getGUID();
        this.ini = ini;
        this.callBack = callBack;
    }

    GetHtml() {
        return getIniConfigGui(this, this.ini, this.GUID, this.callBack);
    }
}

var configContainerGuiTemplate;
function wrapInConfigContainerGui(id, content)
{
    if(!configContainerGuiTemplate)
        configContainerGuiTemplate = getFileContents("ConfigGuiModules/ConfigContainer.html");
    var template = configContainerGuiTemplate;
    template = template.replace(/[$]id[$]/g, id);
    template = template.replace(/[$]content[$]/g, content);
    return template;
}