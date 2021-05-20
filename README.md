# EmbeddedIOServices
This is a unit tested hardware abstraction for embedded systems. The timer service has a built in scheduling system that can be used to trigger callbacks at precise times.

# Interfaces that need to be implemented when adding new hardware abstraction
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

# Currently Supported Microcontroller
<ul>
  <li>STM32F103C8</li>
  <li>Full Stm32 Line, just add Stm32HalConf.h</li>
</ul>

# TODO:
<ul>
  <li>Create Virtual PWM Service using TimerService scheduling rather than built in hardware PWM</li>
  <li>Create integration tests with integration test hardware</li>
</ul>
  
