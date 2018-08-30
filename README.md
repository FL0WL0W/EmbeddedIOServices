# EmbeddedIOServices
This is a unit tested hardware abstraction for embedded systems. The timer service has a built in scheduling system that can be used to trigger callbacks at precise times. The services are configurable for translating raw inputs and outputs to their meaning. I.E. sensor voltage -> sensor value (temperature, pressure, etc.).

TODO:
<ul>
  <li>Create Unit Tests For>
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
  
