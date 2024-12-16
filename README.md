# EmbeddedIOServices
This is a unit tested hardware abstraction for embedded systems. The timer service has a built in scheduling system that can be used to trigger callbacks at precise times. Testing with 32 callbacks being scheduled on a STM32F103, sub microsecond accuracy was observed.

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
    <li>ScheduleCallBack(tick_t tick) <= this is to call ReturnCallBack() at that tick</li>
  </ul>
  <li>ICommunicationService (UART, TCP Socket, WebSocket, etc.)</li>
  <ul>
    <li>Call Receive(void* data, size_t length) when data received</li>
    <li>Send(void* data, size_t length)</li>
  </ul>
  <li>ICANService (UART, TCP Socket, WebSocket, etc.)</li>
  <ul>
    <li>Receive(CANIdentifier_t identifier, CANData_t data, uint8_t dataLength) when data received</li>
    <li>Send(CANIdentifier_t identifier, CANData_t data, uint8_t dataLength)</li>
  </ul>
</ul>

# Currently Supported Microcontroller
<ul>
  <li>STM32F103Cx</li>
  <li>STM32F401CC</li>
  <li>Full Stm32 Line, just add Stm32HalConf.h</li>
  <li>W806 and W801</li>
  <li>ESP32</li>
</ul>
  
