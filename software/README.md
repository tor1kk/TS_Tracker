# Software

## Brief

The primary goal of this software is to receive commands from the SIM800C module, parse these commands, perform various actions (e.g., send GPS coordinates from the NEO-6M module), and send responses via the SIM800C module on the STM32F401RCT6 microcontroller.
For this project, appropriate drivers were written: [HAL-based NEO-6M driver](https://github.com/tor1kk/neo-6m) and [HAL-based SIM800C driver](https://github.com/tor1kk/sim800).


## How its work

After peripheral intitialisation, the current time is set for the RTC peripheral. A global variable (`currentTime`) is initialized to 0, and the program waits for this variable to become non-zero. This variable is modified in the ISR callback after receiving data from the GPS module, where the actual current time is obtained. Afterward, FreeRTOS tasks are created, and the scheduler is started. All tasks are located in the `application_tasks.c` file.

The first task to run is the one that resets and configures the SIM800C module (`vSim800ResetTask`).

Whenever an SMS message is received, the appropriate callback from the SIM800C driver (`SIM800_RcvdSMSCallBack`) is triggered. In this callback, the message and phone number are copied to send this data to a special task (`vCmdProcessTask`) via a queue (`xCommandQueue`). After that, this task is awakened from the blocked state to the ready state. This task parses the message; all commands should be digits, so the received command is compared with the expected commands. If they match, the appropriate action occurs. If the command doesn't match, the message `"Unknown command!"` is transmitted.

Users can send commands to get GPS coordinates, get the voltage on the ADC pin, set the time when coordinates will be received, get a notification for this time, or delete the notification.

### Tasks

- **vSim800ResetTask** - Resets and configures the SIM800 module (priority: `4`)
- **vCmdProcessTask** - Parses received commands (priority: `3`)
- **vSendSMSTask** - Transmits SMS messages (priority: `2`)
- **vConfigRTCAlarmTask** - Configures the RTC alarm (priority: `1`)
- **vSendGPSTask** - Gets the current GPS coordinates and sends them to **vSendSMSTask** (priority: `1`)
- **vSendVoltageTask** - Gets the voltage on the ADC pin and sends it to **vSendSMSTask** (priority: `1`)
- **vSendCurrentAlarmTask** - Gets the current RTC alarm and sends it to **vSendSMSTask** (priority: `1`)


Any data that must be transmitted to the user is sent to the `xSMSToSendQueue`, and `vSendSMSTask` receives this data and sends it to the user.

`vConfigRTCAlarmTask` waits for the time at which the RTC alarm must occur in the `xTimeToRTCAlarmQueue`. It should be noted that when the RTC alarm triggers, the phone number contained in the `phoneNumberForNotify` buffer, which was placed there by `vCmdProcessTask`, is used. This is the number from which the notification command was received.

`vSendGPSTask` waits for numbers to which coordinates must be sent in the `xNumbersToSendGPSQueue`. In this task, the NEO-6M module is powered on and GPS coordinates are awaited in the `xGPSCoordinatesQueue`, which are set in the `NEO6M_RMCCallBack` callback from the NEO-6M driver. Then, an SMS message with the coordinates and number is sent to `vSendSMSTask`.

`vSendVoltageTask` waits for numbers to which the voltage must be sent in the `xNumbersToSendVoltageQueue`. In this task, an ADC conversion is initiated and the voltage value is awaited in the `xADCValueQueue`, which is placed in the `HAL_ADC_ConvCpltCallback`. Then, an SMS message with the voltage and number is sent to `vSendSMSTask`.

`vSendCurrentAlarmTask` waits for a notification from `vCmdProcessTask`. It gets the current RTC alarm and sends this information to the number in the `phoneNumberForNotify` buffer. 

### Block diagram

![GPS tracker FreeRTOS software block diagram](../img/GPS%20tracker%20software.png)






