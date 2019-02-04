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

function getFormulaInput(id, label, values, min, max, step, callBack) {
    var template = "<label>" + label + ":</label>";
    for(var i = values.length-1; i > 0; i--)
    {
        template += "<input id=\"" + id + i + "\" type=\"number\" min=\"" + min + "\" max=\"" + max + "\" step=\"" + step + "\" value=\"" + values[i] + "\"/>";
        template += " x<sup>" + i + "</sup> + ";
    }
    template += "<input id=\"" + id + 0 + "\" type=\"number\" min=\"" + min + "\" max=\"" + max + "\" step=\"" + step + "\" value=\"" + values[0] + "\"/> x";
    
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

    $(document).on("change", "#" + id, function(){
        callBack(this.checked);
    });

    return template;
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