MILTIWII-NRF24
===============

Modified version of the excellent Arduino Multwii brushed motors version to work with nrf24L01 2.4G module.

**Caution** : quadcopter and multi-copter are very dangerous and this software is not fully tested, 
so use this at YOUR OWN RISK !

There are 5 directories :
 * Prototypage board schematic based on atmega328, MPU6050 IMU, nrf24l01 and mosfets for brushed motors.
 * nRF24Manager : nrf24L01 arduino library to ommunicate with 2.4G module (need to be installed as arduino library for next project).
 * MultiWii : modified version of Multwii (version 2.2) to work with nRF24Manager. Principal modified files are RX.ino and Output.ino.
 * RadioControllerMW : arduino transmitter porgram for a PPM remote controller.
 * RadioControllerTest : arduino program to act as receiver to test RadioControllerMW program.

 
Here is a video of V292 quadcopter (brushed motors) using a prototypage board and a Multiwii 2.2 : https://www.youtube.com/watch?v=8ePVVkbjizY

The base used is a V929 on which the electronic is replaced with a micro-controller ATmega328P.
Arduino pro mini module 16Mhz is used as the micro-controller. The voltage regulator is not used because the 3.7V lipo battery is 
connected directly above through capacitors to stabilize it.

The motors are brushed type and are controlled by PWM (256 levels of accuracy) by SI2302 MOSFET where diodes are addes.
Two modules are used as sensors, MPU6050 (accelerometer / gyroscope). 
This also includes the 3.3V voltage regulator supplying 2.4G module (nrf24L01p). 

The radio FM 72 Mhz is modified. The high frequency part is deactivated and replaced by a amplified nRF24L01+ 2.4G module. The PPM signal is decoded 
to retrieve the position of the joysticks. It also contains an arduino pro mini 16Mhz module. The whole thing is powered by a LiPo 7.4V. 
The 5v regulator is included on the arduino module. Another 3.3v regulator is used to supply the module nRF24L01 +. 
The communication protocol is very simple, the state of the sticks is stored across 4 values. The hardware CRC is enabled. 
The frame is sent only once, even if the communication fails because the acknowledgment is disabled.

