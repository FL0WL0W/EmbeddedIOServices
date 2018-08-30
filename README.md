# EmbeddedIOServices
This is a unit tested hardware abstraction for embedded systems. The timer service has a built in scheduling system that can be used to trigger callbacks at precise times. The services are configurable for translating raw inputs and outputs to their meaning. I.E. sensor voltage -> sensor value (temperature, pressure, etc.).

# Interfaces that need to be implemented when adding new hardware
<ul>
  <li>IAnalogService</li>
  <ul>
    <li>InitPin(uint8 pin)</li>
    <li>ReadPin(uint8 pin)</li>
  </ul>
  <li>IDigitalService</li>
  <ul>
    <li>InitPin(uint8 pin, PinDirection direction)</li>
    <li>ReadPin(uint8 pin)</li>
    <li>WritePin(uint8 pin, bool value)</li>
  </ul>
  <li>IPwmService</li>
  <ul>
    <li>InitPin(uint8 pin, PinDirection direction, uint16 minFrequency)</li>
    <li>ReadPin(uint8 pin)</li>
    <li>WritePin(uint8 pin, PwmValue value)</li>
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
    <li>PWM Polynomial Translation</li>
    <li>Static Value</li>
  </ul>
  <li>FloatOutputService</li>
  <ul>
    <li>PWM Polynomial Translation</li>
  </ul>
</ul>

# TODO:
<ul>
  <li>Create Unit Tests For</li>
  <ul>
    <li>BooleanInputService</li>
    <li>BooleanInputService_Static</li>
    <li>FloatInputService_Static</li>
    <li>FloatOutputService_PwmPolynomial</li>
    <li>FloatOutputService_StepperPolynomial</li>
  </ul>
  <li>Move STM32F103C8 Hardware Abstractions over</li>
  <li>Create integration tests on the STM32F103C8</li>
  <li>Create Stepper Services</li>
  <li>Create Lookup Table IO Services</li>
  <li>Create a way to schedule interrupt callbacks for digital inputs.</li>
</ul>
  
