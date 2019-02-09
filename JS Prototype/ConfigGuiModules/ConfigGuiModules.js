var numberConfigGuiTemplate;
function getNumberConfigGui(id, label, value, min, max, step, displayMultiplier, callBack) {
    if(!numberConfigGuiTemplate)
        numberConfigGuiTemplate = getFileContents("ConfigGuiModules/Number.html");
    var template = numberConfigGuiTemplate;
    template = template.replace(/[$]id[$]/g, id);
    template = template.replace(/[$]label[$]/g, label);
    if(displayMultiplier)
        value *= displayMultiplier;
    template = template.replace(/[$]value[$]/g, value);
    template = template.replace(/[$]min[$]/g, min);
    template = template.replace(/[$]max[$]/g, max);
    template = template.replace(/[$]step[$]/g, step);

    $(document).off("change."+id);
    if(callBack) {
        $(document).on("change."+id, "#" + id, function(){
            val = parseFloat($(this).val());
            if(displayMultiplier) 
                val /= displayMultiplier;
            callBack(val);
        });
    }
    return template;
}

function getNumberArrayConfigGui(id, dialog, label, xlabel, zlabel, values, min, max, step, displayMultiplier, callBack, xMin, xMinMin, xMinMax, xMinStep, xMinCallBack, xMax, xMaxMin, xMaxMax, xMaxStep, xMaxCallBack, xMinMaxDisplayMultiplier) {
    var header = "<tr><th>" + xlabel + "</th>";
    var row = "<tr><th>" + zlabel + "</th>";

    if(xMinMaxDisplayMultiplier) {
        xMin *= xMinMaxDisplayMultiplier;
        xMax *= xMinMaxDisplayMultiplier;
    }

    for(var i = 0; i < values.length; i++)
    {
        if(i === 0 && xMinCallBack) {
            header += "<td><input id=\"" + id + "min\" type=\"number\" min=\"" + xMinMin + "\" max=\"" + xMinMax + "\" step=\"" + xMinStep + "\" value=\"" + xMin + "\"/></td>";
        } else if (i === values.length -1 && xMaxCallBack) {
            header += "<td><input id=\"" + id + "max\" type=\"number\" min=\"" + xMaxMin + "\" max=\"" + xMaxMax + "\" step=\"" + xMaxStep + "\" value=\"" + xMax + "\"/></td>";
        } else {
            header += "<td><input id=\"" + id + "x" + i + "\" type=\"number\" disabled value=\"" + parseFloat(parseFloat(((xMax - xMin) * i / (values.length-1) + xMin).toFixed(6)).toPrecision(7)) + "\"/></td>";
        }
        var rowClass = $("#" + id + i).attr("class")
        if(rowClass)
            rowClass = " class =\"" + rowClass + "\"";
        else
            rowClass = "";
        var value = values[i];
        if(displayMultiplier)
            value *= displayMultiplier;
        row += "<td><input id=\"" + id + i + "\" type=\"number\" min=\"" + min + "\" max=\"" + max + "\" step=\"" + step + "\" value=\"" + value + "\""+rowClass+"/></td>";
    }
    
    header += "</tr>"
    row += "</tr>";

    var template = "<table id=\"" + id + "table\" class=\"table2D\">" + header + row + "</table>";
    
    $(document).off("change."+id);
    if(callBack) {
        $.each(values, function(index, value) {
            $(document).on("change."+id, "#" + id + index, function(){
                val = parseFloat($(this).val());
                if(displayMultiplier) 
                    val /= displayMultiplier;
                values[index] = val;
                var selectedCount = 0;
                $.each(values, function(selectedindex, value) { if ($("#" + id + selectedindex).hasClass("selected")) selectedCount++; });
                if(selectedCount > 1) {
                    $.each(values, function(selectedindex, value) {
                        var thisElement = $("#" + id + selectedindex);
                        if(thisElement.hasClass("selected"))  {
                            values[selectedindex] = val;
                        }
                    });
                }
                callBack(values);
            });
        });
    }

    if(xMinCallBack) {
        $(document).on("change."+id, "#" + id + "min", function(){
            val = parseFloat($(this).val());
            if(xMinMaxDisplayMultiplier) 
                val /= xMinMaxDisplayMultiplier;
            xMinCallBack(val);
        });
    }

    if(xMaxCallBack) {
        $(document).on("change."+id, "#" + id + "max", function(){
            val = parseFloat($(this).val());
            if(xMinMaxDisplayMultiplier) 
                val /= xMinMaxDisplayMultiplier;
            xMaxCallBack(val);
        });
    }

    $(document).off("mousedown."+id);
    $(document).off("mouseup."+id);
    $(document).off("mousemove."+id);
    $(document).off("contextmenu."+id);
    $(document).off("copy."+id);
    var selecting = false;
    var pointX;
    $.each(values, function(index, value) {
        $(document).on("mousedown."+id, "#" + id + index, function(){
            pointX =  $(this).offset().left - $(this).closest("table").offset().left;
            $.each(values, function(index, value) {
                $("#" + id + index).removeClass("selected");
            });
            $(this).addClass("selected");
            selecting = true;
        });
        $(document).on("copy."+id, "#" + id + index, function(e){
            var copyData = "";
            $.each(values, function(index, value) {
                if($("#" + id + index).hasClass("selected"))
                    copyData += "\t" + value;
            });
            copyData = copyData.substring(1);
            e.originalEvent.clipboardData.setData('text/plain', copyData);
            e.preventDefault();
        });
        $(document).on("paste."+id, "#" + id + index, function(e){
            var val = e.originalEvent.clipboardData.getData('text/plain');
            $.each(val.split("\t"), function(valIndex, val) {
                if(index + valIndex < values.length) {
                    $("#" + id + (index + valIndex)).addClass("selected");
                    values[index + valIndex] = val;
                }
            });
            callBack(values);
            e.preventDefault();
        });
    });

    $(document).on("mousedown."+id, "#" + id + "table", function(e){
        if(selecting)
            return;
        $.each(values, function(index, value) {
            $("#" + id + index).removeClass("selected");
        });
    });

    $(document).on("mouseup."+id, function(e){
        selecting = false;
    });
    
    $(document).on("mousemove."+id, function(e){
        if(!selecting)
            return;
        $.each(values, function(index, value) {
            var thisElement = $("#" + id + index);
            var thisTable = thisElement.closest("table")
            var relX = e.pageX - thisTable.offset().left;
            var elX = thisElement.offset().left - thisTable.offset().left + (thisElement.width() / 2);
            if((elX <= relX && elX >= pointX) || (elX >= relX && elX <= pointX) || (pointX == thisElement.offset().left - thisTable.offset().left)) {
                thisElement.addClass("selected");
            } else {
                thisElement.removeClass("selected");
            }
        });
    });
    
    $.each(values, function(index, value) {
        $(document).on("contextmenu."+id, "#" + id + index, function(){
            event.preventDefault();
        });
    });

    if(dialog) {
        var style = $("#" + id + "dialog").is(":visible")? "" : "style=\"display: none;\"";
        var buttonVal = $("#" + id + "edit").val();
        if(!buttonVal)
            buttonVal = "Show/Edit";

        template = "<label for=\"" + id + "edit\">" + label + ":</label><input type=\"button\" id=\"" + id + "edit\" value=\""+buttonVal+"\"><div id=\"" + id + "dialog\" "+style+">" +"<span class=\"configElementSpan\">" + template + "</span>" + "</div>";
        
        $(document).off("click."+id);
        $(document).on("click."+id, "#" + id + "edit", function(){
            if($("#" + id + "dialog").is(":visible")) {
                $("#" + id + "dialog").hide();
                $("#" + id + "edit").val("Show/Edit");
            } else {
                $("#" + id + "dialog").show();
                $("#" + id + "edit").val("Hide");
            }
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
    
    $(document).off("change."+id);
    if(callBack) {
        $.each(values, function(index, value) {
            $(document).on("change."+id, "#" + id + index, function(){
                values[index] = $(this).val();
                callBack(values);
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

    $(document).off("change."+id);
    if(callBack) {
        $(document).on("change."+id, "#" + id, function(){
            callBack(this.checked);
        });
    }

    return template;
}

var selectionConfigGuiTemplate;
function getSelectionConfigGui(id, label, value, selections, callBack, clearCallBack) {
    if(!selectionConfigGuiTemplate)
        selectionConfigGuiTemplate = getFileContents("ConfigGuiModules/Selection.html");
    if(selections.length < 2) {
        return "";
    }
    var template = selectionConfigGuiTemplate;
    template = template.replace(/[$]id[$]/g, id);
    template = template.replace(/[$]label[$]/g, label);
    var selectionHtml = "";
    $.each(selections, function(selectionIndex, selectionValue) {
        if(selectionIndex === parseInt(value.Index))
            selectionHtml += "<option selected value=\"" + selectionIndex + "\">" + selectionValue.Name + "</option>";
        else
            selectionHtml += "<option value=\"" + selectionIndex + "\">" + selectionValue.Name + "</option>";
    });
    template = template.replace(/[$]selections[$]/g, selectionHtml);
    
    $(document).off("change."+id);
    if(callBack) {
        $(document).on("change."+id, "#" + id, function(){
            callBack($(this).val());
        });
    }
    $(document).off("click."+id);
    if(clearCallBack) {
        $(document).on("click."+id, "#" + id + "clear", function(){
            clearCallBack();
        });
    }

    return template;
}

function getIniConfigGui(obj, ini, idPrefix, callBack) {
    var template = "<span id=\"span" + idPrefix + "\">"

    var reRender = function() {
        var element = document.activeElement;
        $("#span" + idPrefix).html(obj.GetHtml());
        var newElement = document.getElementById(element.id);
        if(newElement)
            newElement.focus();
    }

    var firstElement = true;

    var addIniRow = function(iniIndex, iniRow){    
        var location = iniGetLocation(obj, iniRow, iniIndex);
        
        if(location === "static")
            return;

        var elementTemplate = "";
        var hideElement = false;

        elementTemplate += "<span class=\"configElementSpan\" id=\"span" + idPrefix + location + "\">"

        var label = iniGetLabel(obj, iniRow);
        var value = iniGetValue(obj, iniRow, iniIndex);
        var min = iniGetMin(obj, iniRow);
        var max = iniGetMax(obj, iniRow);
        var step = iniGetStep(obj, iniRow);

        if(typeof iniRow.Type === "string")
        {
            switch(iniRow.Type.split("[")[0]) {
                case "label":
                    elementTemplate += "<label>" + label + "</label>";
                    break;
                case "bool":
                    elementTemplate += getCheckBoxConfigGui(idPrefix + location, label, value, function(value){
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
                    if(iniRow.Type.split("[").length === 2) {
                        var xMinRef = valueIsReferenceLocation(iniRow.XMin)? ini.find(function(element) { return element.Location === iniRow.XMin}): undefined;
                        var xMaxRef = valueIsReferenceLocation(iniRow.XMin)? ini.find(function(element) { return element.Location === iniRow.XMax}): undefined;
                        if(xMinRef.DisplayMultiplier !== xMaxRef.DisplayMultiplier) {
                            throw "XMin and XMax references do not share the same DisplayMultiplier"
                        }
                        elementTemplate += getNumberArrayConfigGui(idPrefix + location, iniRow.Dialog, label, iniRow.XLabel, iniRow.ZLabel, value, min, max, step, iniRow.DisplayMultiplier, function(value){
                            obj[location] = value;
                            reRender();
                            if(callBack)
                                callBack(obj);
                            }, parseValueString(obj, iniRow.XMin), xMinRef? xMinRef.Min : undefined, xMinRef? xMinRef.Max : undefined, xMinRef? xMinRef.Step : undefined, xMinRef ? function(value){
                                obj[iniRow.XMin] = value;
                                reRender();
                                if(callBack)
                                    callBack(obj);
                            } : undefined, 
                           parseValueString(obj, iniRow.XMax), xMaxRef? xMaxRef.Min : undefined, xMaxRef? xMaxRef.Max : undefined, xMaxRef? xMaxRef.Step : undefined, xMaxRef ? function(value){
                                obj[iniRow.XMax] = value;
                                reRender();
                                if(callBack)
                                    callBack(obj);
                            } : undefined, xMinRef.DisplayMultiplier);
                    } else {
                        var referencedBy = iniReferencedByIniRow(ini, location);
                        if(referencedBy.length !== 1 || (referencedBy[0].XMin !== location && referencedBy[0].XMax !== location && referencedBy[0].YMin !== location && referencedBy[0].YMax !== location)){
                            elementTemplate += getNumberConfigGui(idPrefix + location, label, value, min, max, step, iniRow.DisplayMultiplier, function(value){
                                obj[location] = value;
                                reRender();
                                if(callBack)
                                    callBack(obj);
                            });
                        } else {
                            hideElement = true;
                        }
                    }
                    obj[location] = value;
                    break;
                case "formula":
                    elementTemplate += getFormulaConfigGui(idPrefix + location, label, value, min, max, step, function(value) {
                        obj[location] = value;
                        reRender();
                        if(callBack)
                            callBack(obj);
                    });
                    obj[location] = value;
                    break;
                case "iniselection":
                    elementTemplate += "<span>" + getSelectionConfigGui(idPrefix + location, label, value, iniRow.Selections, function(value) {
                        obj[location].Index = value;
                        if(!obj[location].Value)
                            obj[location] = { Index: value.Index, Value: new ConfigGui(iniRow.Selections[value].ini, function() { reRender(); if(callBack) callBack(obj); })}
                        else
                            obj[location].Value.ini = iniRow.Selections[value].ini
                            reRender();
                        if(callBack)
                            callBack(obj);
                    }, function() {
                        obj[location].Value = new ConfigGui(iniRow.Selections[obj[location].Index].ini, function() { reRender(); if(callBack) callBack(obj); })
                            reRender();
                        if(callBack)
                            callBack(obj);
                    }) + "</span>";
                    if(!value.Value)
                        obj[location] = { Index: value.Index, Value: new ConfigGui(iniRow.Selections[value.Index].ini, function() { reRender(); if(callBack) callBack(obj); })}
                    var innerValue;
                    if(iniRow.WrapInConfigContainer)
                        elementTemplate += "<br>" + wrapInConfigContainerGui("", obj[location].Value.GetHtml());
                    else
                        elementTemplate += obj[location].Value.GetHtml();
                    break;
                case "undeclaredini":
                    iniRow.Type = window[iniRow.UndeclaredType];
                    addIniRow(iniIndex, iniRow);
                    break;
                default:
                    break;
            }
        } else {
            if(!obj[location])
                obj[location] = new ConfigGui(iniRow.Type, function() { reRender(); if(callBack) callBack(obj); });

            if(label) {
                if(iniRow.SameLine) 
                    elementTemplate = "<label for=\"span" + idPrefix + location + "\" class=\"subConfigSameLineLabel\">" + label + ":</label>" + elementTemplate;
                else
                    elementTemplate = "<label for=\"span" + idPrefix + location + "\" class=\"subConfigLabel\">" + label + "</label><span class=\"sameLineSpacer\"></span>" + elementTemplate;
            }
            if(iniRow.WrapInConfigContainer)
                elementTemplate += wrapInConfigContainerGui(obj[location].GUID, obj[location].GetHtml());
            else
                elementTemplate += obj[location].GetHtml();
        }

        elementTemplate += "</span>";

        if(!firstElement) {
            if(iniRow.SameLine) {
                elementTemplate = "<span class=\"sameLineSpacer\"></span>" + elementTemplate;
            } else {
                elementTemplate = "<br>" + elementTemplate;
            }
        }

        if(!hideElement) {
            firstElement = false;
            template += elementTemplate;
        }
    }

    $.each(ini, addIniRow);
    
    template += "</span>";

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