# EmbeddedIOServices
This is a unit tested hardware abstraction for embedded systems. The timer service has a built in scheduling system that can be used to trigger callbacks at precise times. The services are configurable for translating raw inputs and outputs to their meaning. I.E. sensor voltage -> sensor value (temperature, pressure, etc.), Throttle position -> Stepper position.

# Interfaces that need to be implemented when adding new hardware
<ul>
  <li>IAnalogService</li>
  <ul>
    <li>InitPin(uint16 pin)</li>
    <li>ReadPin(uint16 pin)</li>
  </ul>
  <li>IDigitalService</li>
  <ul>
    <li>InitPin(uint16 pin, PinDirection direction)</li>
    <li>ReadPin(uint16 pin)</li>
    <li>WritePin(uint16 pin, bool value)</li>
    <li>ScheduleRecurringInterrupt(uint16 pin, CallBack callBack)
    <li>ScheduleNextInterrupt(uint16 pin, CallBack callBack)
  </ul>
  <li>IPwmService</li>
  <ul>
    <li>InitPin(uint16 pin, PinDirection direction, uint16 minFrequency)</li>
    <li>ReadPin(uint16 pin)</li>
    <li>WritePin(uint16 pin, PwmValue value)</li>
  </ul>
  <li>ITimerService</li>
  <ul>
    <li>GetTick()</li>
    <li>GetTicksPerSecond()</li>
    <li>ScheduleCallBack(uint32 tick) <= this is to call ReturnCallBack() at that tick</li>
  </ul>
</ul>

# Available IO Services
<ul>
  <li>BooleanInputService</li>
  <ul>
    <li>Digital Pin</li>
    <li>Static Value</li>
  </ul>
  <li>BooleanOutputService</li>
  <ul>
    <li>Digital Pin</li>
  </ul>
  <li>FloatInputService</li>
  <ul>
    <li>Analog Polynomial Translation</li>
    <li>Analog Lookup Table Translation</li>
    <li>Frequency Polynomial Translation</li>
    <li>Frequency Lookup Table Translation</li>
    <li>Fault Detection</li>
    <li>Static Value</li>
  </ul>
  <li>FloatOutputService</li>
  <ul>
    <li>PWM Polynomial Translation</li>
    <li>PWM Lookup Table Translation</li>
  </ul>
</ul>

# Currently Supported Microcontroller
<ul>
  <li>STM32F103C8</li>
  <li>Full Stm32 Line, just add Stm32HalConf.h</li>
</ul>

# TODO:
<ul>
  <li>Config Editor</li>
  <li>Create Unit Tests For</li>
  <ul>
    <li>Stepper Services when they are complete</li>
  </ul>
  <li>Create Stepper Services</li>
  <ul>
    <li>Bipolar Stepper</li>
    <li>Step/Direction Stepper</li>
    <li>Stepper Feedback Limit Calibration Wrapper</li>
    <li>Stepper Static Step Calibration Wrapper</li>
  </ul>
  <li>Analog Feedback Float Output Service Wrapper</li>
  <li>Analog Feedback Stepper Float Output Service Wrapper</li>
  <li>Implement STM32Hal interrupt callbacks for digital inputs.</li>
  <li>Create PWM service that uses interrupt callback from DigitalService and Tick from TimerService to create software based PWM input and output.</li>
  <li>*Test Stm32F103 Services</li>
  <li>*Create integration tests with integration test hardware</li>
</ul>
  
