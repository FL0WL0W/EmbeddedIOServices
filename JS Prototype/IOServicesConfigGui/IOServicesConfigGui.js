var BooleanInputService_StaticConfigGuiTemplate;
class BooleanInputService_StaticConfigGui extends BooleanInputService_StaticConfig {
    constructor() {
        super();
        this.Value = false;
    }

    GetHtml() {
        if(!BooleanInputService_StaticConfigGuiTemplate)
            BooleanInputService_StaticConfigGuiTemplate = getFileContents("/IOServicesConfigGui/BooleanInputService_StaticConfigGui.html")
        var template = BooleanInputService_StaticConfigGuiTemplate;
        var thisClass = this;

        template = template.replace(/[$]Value[$]/g, getCheckBoxConfigGui(this.GUID + "Value", "Value", this.Value, function(value){
            thisClass.Value = value;
        }));

        return template;
    }
}

var BooleanInputServiceConfigGuiTemplate;
class BooleanInputServiceConfigGui extends BooleanInputServiceConfig {
    constructor() {
        super();
        this.Pin = 0;
        this.Inverted = false;
    }

    GetHtml() {
        if(!BooleanInputServiceConfigGuiTemplate)
            BooleanInputServiceConfigGuiTemplate = getFileContents("/IOServicesConfigGui/BooleanInputServiceConfigGui.html")
        var template = BooleanInputServiceConfigGuiTemplate;
        var thisClass = this;

        template = template.replace(/[$]Pin[$]/g, getNumberConfigGui(this.GUID + "Pin", "Pin", this.Pin, 0, 65535, 1, function(value){
            thisClass.Pin = value;
        }));

        template = template.replace(/[$]Inverted[$]/g, getCheckBoxConfigGui(this.GUID + "Inverted", "Inverted", this.Inverted, function(value){
            thisClass.Inverted = value;
        }));

        return template;
    }
}

var BooleanOutputServiceConfigGuiTemplate;
class BooleanOutputServiceConfigGui extends BooleanOutputServiceConfig {
    constructor() {
        super();
        this.Pin = 0;
        this.NormalOn = false;
        this.HighZ = false;
    }

    GetHtml() {
        if(!BooleanOutputServiceConfigGuiTemplate)
            BooleanOutputServiceConfigGuiTemplate = getFileContents("/IOServicesConfigGui/BooleanOutputServiceConfigGui.html")
        var template = BooleanOutputServiceConfigGuiTemplate;
        var thisClass = this;

        template = template.replace(/[$]Pin[$]/g, getNumberConfigGui(this.GUID + "Pin", "Pin", this.Pin, 0, 65535, 1, function(value){
            thisClass.Pin = value;
        }));

        template = template.replace(/[$]NormalOn[$]/g, getCheckBoxConfigGui(this.GUID + "NormalOn", "NormalOn", this.NormalOn, function(value){
            thisClass.NormalOn = value;
        }));

        template = template.replace(/[$]HighZ[$]/g, getCheckBoxConfigGui(this.GUID + "HighZ", "HighZ", this.HighZ, function(value){
            thisClass.HighZ = value;
        }));

        return template;
    }
}

class ButtonService_PollingConfigGui extends ButtonService_PollingConfig {
    
}

var FloatInputService_StaticGuiTemplate;
class FloatInputService_StaticConfigGui extends FloatInputService_Static {
    constructor() {
        super();
        this.Value = 0;
        this.ValueDot = 0;
    }

    GetHtml() {
        if(!FloatInputService_StaticGuiTemplate)
            FloatInputService_StaticGuiTemplate = getFileContents("/IOServicesConfigGui/FloatInputService_StaticConfigGui.html")
        var template = FloatInputService_StaticGuiTemplate;
        var thisClass = this;

        template = template.replace(/[$]Value[$]/g, getNumberConfigGui(this.GUID + "Value", "Value", this.Value, -1000000, 1000000, 0.01, function(value){
            thisClass.Value = value;
        }));

        template = template.replace(/[$]ValueDot[$]/g, getNumberConfigGui(this.GUID + "ValueDot", "ValueDot", this.ValueDot, -1000000, 1000000, 0.01, function(value){
            thisClass.ValueDot = value;
        }));

        return template;
    }
}

var FloatInputService_AnalogPolynomialConfigGuiTemplate;
class FloatInputService_AnalogPolynomialConfigGui extends FloatInputService_AnalogPolynomialConfig {
    constructor(Degree) {
        super(Degree);
        this.GUID = getGUID();
        this.MinValue = 0;
        this.MaxValue = 0;
        this.DotSampleRate = 1000;
    }

    GetHtml() {
        if(!FloatInputService_AnalogPolynomialConfigGuiTemplate)
            FloatInputService_AnalogPolynomialConfigGuiTemplate = getFileContents("/IOServicesConfigGui/FloatInputService_AnalogPolynomialConfigGui.html")
        var template = FloatInputService_AnalogPolynomialConfigGuiTemplate;
        var thisClass = this;

        template = template.replace(/[$]A[$]/g, getFormulaInput(this.GUID + "A", "Coefficients", this.A, -1000000, 1000000, 0.01, function(index, value) {
            thisClass.A[index] = value;
        }));

        template = template.replace(/[$]MinValue[$]/g, getNumberConfigGui(this.GUID + "MinValue", "Min Value", this.MinValue, -1000000, 1000000, 0.01, function(value){
            thisClass.MinValue = value;
        }));

        template = template.replace(/[$]MaxValue[$]/g, getNumberConfigGui(this.GUID + "MaxValue", "Max Value", this.MaxValue, -1000000, 1000000, 0.01, function(value){
            thisClass.MaxValue = value;
        }));

        template = template.replace(/[$]DotSampleRate[$]/g, getNumberConfigGui(this.GUID + "DotSampleRate", "Dot Sample Rate", this.DotSampleRate, 1, 65535, 1, function(value){
            thisClass.DotSampleRate = value;
        }));

        return template;
    }
}