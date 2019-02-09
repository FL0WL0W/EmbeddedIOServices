var IOServicesIni = {};

IOServicesIni.BooleanInputService_StaticConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "Value", Type: "bool", Label: "Value", DefaultValue: false }
];

IOServicesIni.BooleanInputServiceConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 2 },
    { Location: "Pin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "Inverted", Type: "bool", Label: "Inverted", DefaultValue: false }
];

IOServicesIni.IBooleanInputServiceConfig = [
    { Location: "Selection", Type: "iniselection", Label: "Input", DefaultValue: {Index: 1}, WrapInConfigContainer: false, Selections: [
        { Name: "Static", Ini: "BooleanInputService_StaticConfig"},
        { Name: "Pin", Ini: "BooleanInputServiceConfig"}
    ] }
];

IOServicesIni.BooleanOutputServiceConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "Pin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "NormalOn", Type: "bool", Label: "Normal On", DefaultValue: false },
    { Location: "HighZ", Type: "bool", Label: "High Z", DefaultValue: false }
];

IOServicesIni.IBooleanOutputServiceConfig = [
    { Location: "Selection", Type: "iniselection", Label: "Output", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "Pin", Ini: "BooleanOutputServiceConfig"}
    ] }
];

IOServicesIni.ButtonService_PollingConfig  = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "BooleanInputServiceConfig", Ini: "IBooleanInputServiceConfig" }
];

IOServicesIni.IButtonServiceConfig  = [
    { Location: "Selection", Type: "iniselection", Label: "Output", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "Polling", Ini: "ButtonService_PollingConfig"}
    ] }
];

IOServicesIni.FloatInputService_StaticConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "Value", Type: "float", Label: "Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "ValueDot", Type: "float", Label: "Value Dot", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 }
];

IOServicesIni.FloatInputService_AnalogPolynomialConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 2 },
    { Location: "AdcPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "DotSampleRate", Type: "uint16", Label: "Dot Sample Rate", DefaultValue: 1000, Min: 1, Max: 65535, Step: 1 },
    { Location: "A", Type: "formula[4]", Label: "Coefficients", DefaultValue: 0 },
    { Location: "MinValue", Type: "float", Label: "Min Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "MaxValue", Type: "float", Label: "Max Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 }
];

IOServicesIni.FloatInputService_AnalogInterpolatedTableConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 4 },
    { Location: "AdcPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "DotSampleRate", Type: "uint16", Label: "Dot Sample Rate", DefaultValue: 1000, Min: 1, Max: 65535, Step: 1 },
    { Location: "MinInputValue", Type: "float", Label: "Min Input Voltage", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1  },
    { Location: "MaxInputValue", Type: "float", Label: "Max Input Voltage", DefaultValue: 3.3, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "Resolution", Type: "uint8", Label: "Resolution", DefaultValue: 8, Min: 1, Max: 255, Step: 1 },
    { Location: "Table", Type: "float[Resolution]", Label: "Input Voltage to Value", XLabel: "Input Voltage", ZLabel: "Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1, XMin: "MinInputValue", XMax: "MaxInputValue", Dialog: true }
];

IOServicesIni.FloatInputService_FrequencyPolynomialConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 3 },
    { Location: "PwmPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "MinFrequency", Type: "uint16", Label: "Min Frequency", DefaultValue: 50, Min: 1, Max: 65535, Step: 1 },
    { Location: "DotSampleRate", Type: "uint16", Label: "Dot Sample Rate", DefaultValue: 1000, Min: 1, Max: 65535, Step: 1 },
    { Location: "A", Type: "formula[4]", Label: "Coefficients", DefaultValue: 0 },
    { Location: "MinValue", Type: "float", Label: "Min Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "MaxValue", Type: "float", Label: "Max Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 }
];

IOServicesIni.FloatInputService_FrequencyInterpolatedTableConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 5 },
    { Location: "PwmPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "DotSampleRate", Type: "uint16", Label: "Dot Sample Rate", DefaultValue: 1000, Min: 1, Max: 65535, Step: 1 },
    { Location: "MinFrequency", Type: "uint16", Label: "Min Frequency", DefaultValue: 50, Min: 1, Max: 65535, Step: 1 },
    { Location: "MaxFrequency", Type: "uint16", Label: "Max Frequency", DefaultValue: 100, Min: 1, Max: 65535, Step: 1 },
    { Location: "Resolution", Type: "uint8", Label: "Resolution", DefaultValue: 11, Min: 1, Max: 255, Step: 1 },
    { Location: "Table", Type: "float[Resolution]", Label: "Input Voltage to Value", XLabel: "Input Voltage", ZLabel: "Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1, XMin: "MinFrequency", XMax: "MaxFrequency", Dialog: true }
];

IOServicesIni.IFloatInputServiceConfig = [
    { Location: "Selection", Type: "iniselection", Label: "Input", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "Static", Ini: "FloatInputService_StaticConfig"},
        { Name: "Analog Pin Polynomial", Ini: "FloatInputService_AnalogPolynomialConfig"},
        { Name: "Analog Pin Lookup Table", Ini: "FloatInputService_AnalogInterpolatedTableConfig"},
        { Name: "Frequency Pin Polynomial", Ini: "FloatInputService_FrequencyPolynomialConfig"},
        { Name: "Frequency Pin Lookup Table", Ini: "FloatInputService_FrequencyInterpolatedTableConfig"}
    ] }
];

IOServicesIni.StepperOutputService_StepDirectionControlConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "MaxStepsPerSecond", Type: "uint16", Label: "Steps/Second", DefaultValue: 100, Min: 1, Max: 65535, Step: 1 },
    { Location: "StepWidth", Type: "float", Label: "Step Pulse Width (ms)", DefaultValue: 0.005, Min: 0, Max: 1, Step: 0.001, DisplayMultiplier: 1000 },
    { Location: "StepBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "Step Output Config"},
    { Location: "DirectionBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "Direction Output Config"}
];

IOServicesIni.StepperOutputService_FullStepControlConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 2 },
    { Location: "MaxStepsPerSecond", Type: "uint16", Label: "Steps/Second", DefaultValue: 100, Min: 1, Max: 65535, Step: 1 },
    { Location: "StepWidth", Type: "float", Label: "Step Pulse Width (ms)", DefaultValue: 0.005, Min: 0, Max: 1, Step: 0.001, DisplayMultiplier: 1000 },
    { Location: "APlusBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "A+"},
    { Location: "AMinusBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "A-"},
    { Location: "BPlusBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "B+"},
    { Location: "BMinusBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "B-"}
];

IOServicesIni.StepperOutputService_HalfStepControlConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 3 },
    { Location: "MaxStepsPerSecond", Type: "uint16", Label: "Steps/Second", DefaultValue: 100, Min: 1, Max: 65535, Step: 1 },
    { Location: "StepWidth", Type: "float", Label: "Step Pulse Width (ms)", DefaultValue: 0.005, Min: 0, Max: 1, Step: 0.001, DisplayMultiplier: 1000 },
    { Location: "APlusBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "A+"},
    { Location: "AMinusBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "A-"},
    { Location: "BPlusBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "B+"},
    { Location: "BMinusBooleanOutputServiceConfig", Ini: "IBooleanOutputServiceConfig", Label: "B-"}
];

IOServicesIni.StepperOutputService_StaticStepCalibrationWrapperConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 5 },
    { Location: "StepsOnCalibration", Type: "int32", Label: "Steps/Second", DefaultValue: 300, Min: 2147483648, Max: 2147483647, Step: 1 },
    { Location: "StepperConfig", Ini: "IStepperOutputServiceConfig", Label: "" }
];

IOServicesIni.IStepperOutputServiceConfig = [
    { Location: "Selection", Type: "iniselection", Label: "Input", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "Step Direction", Ini: "StepperOutputService_StepDirectionControlConfig"},
        { Name: "Full Step Coil Control", Ini: "StepperOutputService_FullStepControlConfig"},
        { Name: "Half Step Coil Control", Ini: "StepperOutputService_HalfStepControlConfig"},
        { Name: "Step Calibration Wrapper", Ini: "StepperOutputService_StaticStepCalibrationWrapperConfig"}
    ] }
];

IOServicesIni.FloatOutputService_PwmPolynomialConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 1 },
    { Location: "PwmPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "Frequency", Type: "uint16", Label: "Frequency", DefaultValue: 50, Min: 1, Max: 65535, Step: 1 },
    { Location: "A", Type: "formula[4]", Label: "Coefficients", DefaultValue: 0 },
    { Location: "MinDutyCycle", Type: "float", Label: "Min Duty Cycle", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01 },
    { Location: "MaxDutyCycle", Type: "float", Label: "Max Duty Cycle", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01 }
];

IOServicesIni.FloatOutputService_PwmInterpolatedTableConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 3 },
    { Location: "PwmPin", Type: "uint16", Label: "Pin", DefaultValue: 0, Min: 0, Max: 65535, Step: 1 },
    { Location: "Frequency", Type: "uint16", Label: "Frequency", DefaultValue: 50, Min: 1, Max: 65535, Step: 1 },
    { Location: "MinValue", Type: "float", Label: "Min Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "MaxValue", Type: "float", Label: "Max Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "Resolution", Type: "uint8", Label: "Resolution", DefaultValue: 8, Min: 1, Max: 255, Step: 1 },
    { Location: "Table", Type: "float[Resolution]", Label: "Value to Duty Cycle", XLabel: "Value", ZLabel: "Duty Cycle", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01, XMin: "MinValue", XMax: "MaxValue", Dialog: true }
];

IOServicesIni.FloatOutputService_StepperPolynomialConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 2 },
    { Location: "A", Type: "formula[4]", Label: "Coefficients", DefaultValue: 0 },
    { Location: "MinStepPosition", Type: "float", Label: "Min Step Position", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01 },
    { Location: "MaxStepPosition", Type: "float", Label: "Max Step Position", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01 },
    { Location: "StepperConfig", Ini: "IStepperOutputServiceConfig" }
];

IOServicesIni.FloatOutputService_StepperInterpolatedTableConfig = [
    { Location: "static", Type: "uint8", DefaultValue: 4 },
    { Location: "MinValue", Type: "float", Label: "Min Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "MaxValue", Type: "float", Label: "Max Value", DefaultValue: 0, Min: -1000000, Max: 1000000, Step: 0.1 },
    { Location: "Resolution", Type: "uint8", Label: "Resolution", DefaultValue: 8, Min: 1, Max: 255, Step: 1 },
    { Location: "Table", Type: "float[Resolution]", Label: "Value to Steps", XLabel: "Value", ZLabel: "Steps", DefaultValue: 0, Min: 0, Max: 1, Step: 0.01, XMin: "MinValue", XMax: "MaxValue", Dialog: true },
    { Location: "StepperConfig", Ini: "IStepperOutputServiceConfig" }
];

IOServicesIni.IFloatOutputServiceConfig = [
    { Location: "Selection", Type: "iniselection", Label: "Output", DefaultValue: {Index: 0}, WrapInConfigContainer: false, Selections: [
        { Name: "PWM Pin Polynomial", Ini: "FloatOutputService_PwmPolynomialConfig"},
        { Name: "PWM Pin Lookup Table", Ini: "FloatOutputService_PwmInterpolatedTableConfig"},
        { Name: "Stepper Polynomial", Ini: "FloatOutputService_StepperPolynomialConfig"},
        { Name: "Stepper Lookup Table", Ini: "FloatOutputService_StepperInterpolatedTableConfig"}
    ] }
];

IOServicesIni.Main = [
    { Location: "BooleanInputService", Ini: "IBooleanInputServiceConfig", Label: "BooleanInputService", WrapInConfigContainer: true },
    { Location: "ButtonService", Ini: "IButtonServiceConfig", Label: "ButtonService", WrapInConfigContainer: true },
    { Location: "BooleanOutputService", Ini: "IBooleanOutputServiceConfig", Label: "BooleanOutputService", WrapInConfigContainer: true },
    { Location: "FloatInputService", Ini: "IFloatInputServiceConfig", Label: "FloatInputService", WrapInConfigContainer: true },
    { Location: "StepperOutputService", Ini: "IStepperOutputServiceConfig", Label: "StepperOutputService", WrapInConfigContainer: true },
    { Location: "FloatOutputService", Ini: "IFloatOutputServiceConfig", Label: "FloatOutputService", WrapInConfigContainer: true }
];