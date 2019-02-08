var BooleanInputService_StaticConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "Value", Type: "bool", Label: "Value", DefaultValue: false }
];

var BooleanInputServiceConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 2 },
    { Location: "Pin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "Inverted", Type: "bool", Label: "Inverted", DefaultValue: false }
];

IBooleanInputServiceConfigIni = [
    { Location: "Selection", Type: "iniselection", Label: "Input", DefaultValue: {Index: 1}, WrapInConfigContainer: false, Selections: [
        { Name: "Static",  ini: BooleanInputService_StaticConfigIni},
        { Name: "Pin",  ini: BooleanInputServiceConfigIni}
    ] }
];

var BooleanOutputServiceConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "Pin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "NormalOn", Type: "bool", Label: "Normal On", DefaultValue: false },
    { Location: "HighZ", Type: "bool", Label: "High Z", DefaultValue: false }
];

var ButtonService_PollingConfigIni  = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "BooleanInputServiceConfig", Type: IBooleanInputServiceConfigIni, Label: "Boolean Input Config"}
];

var FloatInputService_StaticConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "Value", Type: "float", Label: "Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "ValueDot", Type: "float", Label: "Value Dot", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 }
];

var FloatInputService_AnalogPolynomialConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 2 },
    { Location: "AdcPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "DotSampleRate", Type: "uint16", Label: "Dot Sample Rate", DefaultValue: 1000, Min: 1, Max: 65535, Step: 1 },
    { Location: "A", Type: "formula[4]", Label: "Coefficients", DefaultValue: 0 },
    { Location: "MinValue", Type: "float", Label: "Min Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "MaxValue", Type: "float", Label: "Max Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 }
];

var FloatInputService_AnalogInterpolatedTableConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 4 },
    { Location: "AdcPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "DotSampleRate", Type: "uint16", Label: "Dot Sample Rate", DefaultValue: 1000, Min: 1, Max: 65535, Step: 1 },
    { Location: "MinInputValue", Type: "float", Label: "Min Input Voltage", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "MaxInputValue", Type: "float", Label: "Max Input Voltage", DefaultValue: 3.3, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "Resolution", Type: "uint8", Label: "Resolution", DefaultValue: 8, Min: 1, Max: 255, Step: 1 },
    { Location: "Table", Type: "float[Resolution]", Label: "Input Voltage to Value", XLabel: "Input Voltage", ZLabel: "Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1, XMin: "MinInputValue", XMax: "MaxInputValue", Dialog: true }
];

var FloatInputService_FrequencyPolynomialConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 3 },
    { Location: "PwmPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "MinFrequency", Type: "uint16", Label: "Min Frequency", DefaultValue: 50, Min: 1, Max: 65535, Step: 1 },
    { Location: "DotSampleRate", Type: "uint16", Label: "Dot Sample Rate", DefaultValue: 1000, Min: 1, Max: 65535, Step: 1 },
    { Location: "A", Type: "formula[4]", Label: "Coefficients", DefaultValue: 0 },
    { Location: "MinValue", Type: "float", Label: "Min Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "MaxValue", Type: "float", Label: "Max Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 }
];

var FloatInputService_FrequencyInterpolatedTableConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 5 },
    { Location: "PwmPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "DotSampleRate", Type: "uint16", Label: "Dot Sample Rate", DefaultValue: 1000, Min: 1, Max: 65535, Step: 1 },
    { Location: "MinFrequency", Type: "uint16", Label: "Min Frequency", DefaultValue: 50, Min: 1, Max: 65535, Step: 1 },
    { Location: "MaxFrequency", Type: "uint16", Label: "Max Frequency", DefaultValue: 100, Min: 1, Max: 65535, Step: 1 },
    { Location: "Resolution", Type: "uint8", Label: "Resolution", DefaultValue: 11, Min: 1, Max: 255, Step: 1 },
    { Location: "Table", Type: "float[Resolution]", Label: "Input Voltage to Value", XLabel: "Input Voltage", ZLabel: "Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1, XMin: "MinFrequency", XMax: "MaxFrequency", Dialog: true }
];

IFloatInputServiceIni = [
    { Location: "Selection", Type: "iniselection", Label: "Input", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "Static",  ini: FloatInputService_StaticConfigIni},
        { Name: "Analog Pin Polynomial",  ini: FloatInputService_AnalogPolynomialConfigIni},
        { Name: "Analog Pin Lookup Table",  ini: FloatInputService_AnalogInterpolatedTableConfigIni},
        { Name: "Frequency Pin Polynomial",  ini: FloatInputService_FrequencyPolynomialConfigIni},
        { Name: "Frequency Pin Lookup Table",  ini: FloatInputService_FrequencyInterpolatedTableConfigIni}
    ] }
];

var testbuilder = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "MAPConfig", Type: IFloatInputServiceIni, Label: "MAP", WrapInConfigContainer: true },
    { Location: "MAFConfig", Type: IFloatInputServiceIni, Label: "MAF", WrapInConfigContainer: true },
    { Location: "TPSConfig", Type: IFloatInputServiceIni, Label: "TPS", WrapInConfigContainer: true }
]