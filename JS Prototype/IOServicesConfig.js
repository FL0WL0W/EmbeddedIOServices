var BooleanInputService_StaticConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "Value", Type: "bool", Label: "Value", DefaultValue: false }
];

var BooleanInputServiceConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 2 },
    { Location: "Pin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "Inverted", Type: "bool", Label: "Inverted", DefaultValue: false }
];

var IBooleanInputServiceConfigIni = [
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

var IBooleanOutputServiceConfigIni = [
    { Location: "Selection", Type: "iniselection", Label: "Output", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "Pin",  ini: BooleanOutputServiceConfigIni}
    ] }
];

var ButtonService_PollingConfigIni  = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "BooleanInputServiceConfig", Type: IBooleanInputServiceConfigIni }
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
    { Location: "MinInputValue", Type: "float", Label: "Min Input Voltage", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1  },
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

var IFloatInputServiceConfigIni = [
    { Location: "Selection", Type: "iniselection", Label: "Input", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "Static",  ini: FloatInputService_StaticConfigIni},
        { Name: "Analog Pin Polynomial",  ini: FloatInputService_AnalogPolynomialConfigIni},
        { Name: "Analog Pin Lookup Table",  ini: FloatInputService_AnalogInterpolatedTableConfigIni},
        { Name: "Frequency Pin Polynomial",  ini: FloatInputService_FrequencyPolynomialConfigIni},
        { Name: "Frequency Pin Lookup Table",  ini: FloatInputService_FrequencyInterpolatedTableConfigIni}
    ] }
];

var StepperOutputService_StepDirectionControlConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "MaxStepsPerSecond", Type: "uint16", Label: "Steps/Second", DefaultValue: 100, Min: 1, Max: 65535, Step: 1 },
    { Location: "StepWidth", Type: "float", Label: "Step Pulse Width (ms)", DefaultValue: 0.005, Min: 0, Max: 1, Step: 0.001, DisplayMultiplier: 1000 },
    { Location: "StepBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "Step Output Config"},
    { Location: "DirectionBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "Direction Output Config"}
];

var StepperOutputService_FullStepControlConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 2 },
    { Location: "MaxStepsPerSecond", Type: "uint16", Label: "Steps/Second", DefaultValue: 100, Min: 1, Max: 65535, Step: 1 },
    { Location: "StepWidth", Type: "float", Label: "Step Pulse Width (ms)", DefaultValue: 0.005, Min: 0, Max: 1, Step: 0.001, DisplayMultiplier: 1000 },
    { Location: "APlusBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "A+"},
    { Location: "AMinusBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "A-"},
    { Location: "BPlusBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "B+"},
    { Location: "BMinusBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "B-"}
];

var StepperOutputService_HalfStepControlConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 3 },
    { Location: "MaxStepsPerSecond", Type: "uint16", Label: "Steps/Second", DefaultValue: 100, Min: 1, Max: 65535, Step: 1 },
    { Location: "StepWidth", Type: "float", Label: "Step Pulse Width (ms)", DefaultValue: 0.005, Min: 0, Max: 1, Step: 0.001, DisplayMultiplier: 1000 },
    { Location: "APlusBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "A+"},
    { Location: "AMinusBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "A-"},
    { Location: "BPlusBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "B+"},
    { Location: "BMinusBooleanOutputServiceConfig", Type: IBooleanOutputServiceConfigIni, Label: "B-"}
];

var StepperOutputService_StaticStepCalibrationWrapperConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 5 },
    { Location: "StepsOnCalibration", Type: "int32", Label: "Steps/Second", DefaultValue: 300, Min: 2147483648, Max: 2147483647, Step: 1 },
    { Location: "StepperConfig", Type: "undeclaredini", UndeclaredType: "IStepperOutputServiceConfigIni", Label: "" } // has to be undeclared type since this hasn't been selected yet
];

var IStepperOutputServiceConfigIni = [
    { Location: "Selection", Type: "iniselection", Label: "Input", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "Step Direction",  ini: StepperOutputService_StepDirectionControlConfigIni},
        { Name: "Full Step Coil Control",  ini: StepperOutputService_FullStepControlConfigIni},
        { Name: "Half Step Coil Control",  ini: StepperOutputService_HalfStepControlConfigIni},
        { Name: "Static Step Calibration Wrapper",  ini: StepperOutputService_StaticStepCalibrationWrapperConfig}
    ] }
];

//wrappers have to have the child assigned to them after it is declared

var FloatOutputService_PwmPolynomialConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "PwmPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "Frequency", Type: "uint16", Label: "Frequency", DefaultValue: 50, Min: 1, Max: 65535, Step: 1 },
    { Location: "A", Type: "formula[4]", Label: "Coefficients", DefaultValue: 0 },
    { Location: "MinDutyCycle", Type: "float", Label: "Min Duty Cycle", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01 },
    { Location: "MaxDutyCycle", Type: "float", Label: "Max Duty Cycle", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01 }
];

var FloatOutputService_PwmInterpolatedTableConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 3 },
    { Location: "PwmPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "Frequency", Type: "uint16", Label: "Frequency", DefaultValue: 50, Min: 1, Max: 65535, Step: 1 },
    { Location: "MinValue", Type: "float", Label: "Min Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "MaxValue", Type: "float", Label: "Max Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "Resolution", Type: "uint8", Label: "Resolution", DefaultValue: 8, Min: 1, Max: 255, Step: 1 },
    { Location: "Table", Type: "float[Resolution]", Label: "Value to Duty Cycle", XLabel: "Value", ZLabel: "Duty Cycle", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01, XMin: "MinValue", XMax: "MaxValue", Dialog: true }
];

var FloatOutputService_StepperPolynomialConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 2 },
    { Location: "A", Type: "formula[4]", Label: "Coefficients", DefaultValue: 0 },
    { Location: "MinStepPosition", Type: "float", Label: "Min Step Position", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01 },
    { Location: "MaxStepPosition", Type: "float", Label: "Max Step Position", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01 },
    { Location: "StepperConfig", Type: IStepperOutputServiceConfigIni }
];

var FloatOutputService_StepperInterpolatedTableConfigIni = [
    { Location: "static", Type: "uint8", DefaultValue: 4 },
    { Location: "MinValue", Type: "float", Label: "Min Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "MaxValue", Type: "float", Label: "Max Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "Resolution", Type: "uint8", Label: "Resolution", DefaultValue: 8, Min: 1, Max: 255, Step: 1 },
    { Location: "Table", Type: "float[Resolution]", Label: "Value to Steps", XLabel: "Value", ZLabel: "Steps", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01, XMin: "MinValue", XMax: "MaxValue", Dialog: true },
    { Location: "StepperConfig", Type: IStepperOutputServiceConfigIni }
];

IFloatOutputServiceConfigIni = [
    { Location: "Selection", Type: "iniselection", Label: "Output", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "PWM Pin Polynomial",  ini: FloatOutputService_PwmPolynomialConfigIni},
        { Name: "PWM Pin Lookup Table",  ini: FloatOutputService_PwmInterpolatedTableConfigIni},
        { Name: "Stepper Polynomial",  ini: FloatOutputService_StepperPolynomialConfigIni},
        { Name: "Stepper Lookup Table",  ini: FloatOutputService_StepperInterpolatedTableConfigIni}
    ] }
];

var testbuilder = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "MAPConfig", Type: IFloatInputServiceConfigIni, Label: "MAP", WrapInConfigContainer: true },
    { Location: "MAFConfig", Type: IFloatInputServiceConfigIni, Label: "MAF", WrapInConfigContainer: true },
    { Location: "TPSConfig", Type: IFloatInputServiceConfigIni, Label: "TPS", WrapInConfigContainer: true },
    { Location: "IACConfig", Type: IFloatOutputServiceConfigIni, Label: "IAC", WrapInConfigContainer: true }
]