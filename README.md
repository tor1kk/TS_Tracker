# TheftShield_Tracker - GPS tracker with SMS communication 


## Brief

TheftShield Tracker is a device designed to provide real-time tracking and monitoring capabilities for valuable assets or vehicles. It leverages the power of GPS technology, GSM communication, and a microcontroller to offer a comprehensive solution for theft prevention and location tracking.

## Usage Instructions

To use the TheftShield Tracker, follow these steps:

1. **Insert SIM Card**:
    - Insert a nano SIM card into the SIM card slot of the device. Ensure that the SIM card does not have a PIN code set.

2. **Restart the Module**:
    - Restart the module by shorting the NRST and GND pins. This will reset the device and prepare it for operation.

3. **Wait for GPS Initialization**:
    - After restarting, the module will wait until the GPS module acquires a valid location fix. This might take a few minutes, especially in a new location or if the device has been off for a long time.

4. **GSM Module Network Registration**:
    - The GSM module will begin registering with the network. You can monitor the registration process by observing the LED indicator on the module:
        - **Fast blinking** (once every second): The module is searching for or trying to connect to a network.
        - **Slow blinking** (once every 3 seconds): The module is connected to the network.

5. **Send Commands**:
    - Once the device is connected to the network, you can start sending SMS commands to it. Note that the first command you send is usually ignored, so you might need to send it a second time.
    
## Features

The TheftShield Tracker offers the following key features:

- **GPS Tracking**: The device is equipped with a NEO-6M GPS module from u-blox, allowing it to accurately determine its geographic coordinates (latitude and longitude). The NEO-6M is a high-performance GPS receiver with excellent sensitivity and low power consumption.
- **GSM Communication**: The SIM800C GSM module from SIMCOM enables the device to send and receive SMS messages, facilitating remote control and data transmission. The SIM800C is a quad-band GSM/GPRS module that supports voice, SMS, and data transfer.
- **Real-Time Location Updates**: Upon receiving an SMS command, the device can promptly send its current GPS coordinates to a specified phone number.
- **Battery Voltage Monitoring**: The device can measure and report the battery voltage level to a specified phone number, ensuring timely monitoring of the power supply. It uses the built-in Analog-to-Digital Converter (ADC) of the STM32F401RCT6 microcontroller to measure the battery voltage.
- **RTC Alarm Configuration**: Users can set an alarm time through an SMS command. When the alarm triggers, the device will automatically send its GPS coordinates to a predefined phone number. The device utilizes the Real-Time Clock (RTC) peripheral of the STM32F401RCT6 microcontroller to set and manage the alarm.
- **Alarm Time Management**: The device allows users to request the current configured alarm time or deactivate the alarm through SMS commands.
- **Power Management**: The device can enter low-power sleep mode when idle, conserving battery life and extending operational duration. The STM32F401RCT6 microcontroller supports various low-power modes, allowing the device to minimize power consumption when not actively performing tasks.

**Here's a table of the supported SMS commands:**

| Command              | Format           | Description                                                                                         |
|----------------------|------------------|-----------------------------------------------------------------------------------------------------|
| Get GPS Coordinates  | 1:               | Sends the device's current GPS coordinates to the specified phone number.                           |
| Get Voltage          | 2:               | Sends the battery voltage level to the specified phone number.                                      |
| Set Alarm Time       | 3:time:          | Sets the RTC alarm time in 24-hour format (e.g., 3:2359: for 11:59 PM). The device will send GPS coordinates to the specified phone number when the alarm triggers. |
| Get Alarm Time       | 4:               | Requests the currently configured RTC alarm time.                                                   |
| Deactivate Alarm     | 5:               | Deactivates the RTC alarm.                                                                          |

The device is powered by a backup battery and incorporates power management features to extend its operational lifetime. It utilizes the FreeRTOS real-time operating system to manage tasks and ensure efficient resource allocation.

With its combination of GPS tracking, GSM communication, and remote control capabilities, the TheftShield Tracker offers a reliable and versatile solution for asset tracking, theft prevention, and location monitoring applications.

## Photos

### 3D Model

![3D Model GPS tracker front view](/img/3d_top_view.png)
![3D Model GPS tracker bottom view](/img/3d_bottom_view.png)

### Real PCB

![Real PCB GPS tracker front view](/img/top_view.png)<br/> 
![Real PCB GPS tracker bottom view](/img/bottom_view.png) 

# Project Structure

The project is organized into two main parts:

TheftShield_Tracker/<br/>
├── hardware/<br/>
│ └── README.md<br/>
└── software/<br/>
│└── README.md<br/>
└──
### hardware/
- **README.md**: Contains information about the hardware design.

### software/
- **README.md**: Contains information about the software implementation.
