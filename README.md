# IOT-Home-Automation
Purpose of the project to Remote control MP3 player in home through internet using ESP8266 Wemos D1

# Things Needed

1. ESP8266 Wemos D1 board
2. Jummper Wire
3. Micro USB Cable
4. Arduino IDE
5. DF Player
6. Micro SD Card
7. 1k Resister

# 1. ESP8266 Wemos D1 Setup
  Refer my previous Project https://github.com/Sudhakar85/IOT-ESP8266 for Arduino IDE and ESP8266 Setup for programming


# 2. Setup SD Card
Format the SD Card and create the folder name like 01, 11, 31 , 99 etc and upload the MP3 files with name like 001.mp3, 002.m03.
The file name can't be long since file selection is based on the command passed to the DF Player

# 3. Connections

Follow the below PIN diagram and setup the connection

![Diagram](/Diagram.jpg?raw=true "Diagram")

# 4. HOW DF Player Works

Below are the specification and PIN details.

Specification:

         Item                                                 Description
                                     1、Support 11172-3 and ISO13813-3 layer3 audio decoding
      MP3Format                  2、Support sampling rate (KHZ):8/11.025/12/16/22.05/24/32/44.1/48
                                         3、Support Normal、Jazz、Classic、Pop、Rock etc
      UART Port         Standard Serial; TTL Level; Baud rate adjustable(default baud rate is 9600)
 Working Voltage                                     DC3.2~5.0V Type DC4.2V
 Standby Current                                                 20mA
       Operating
                                                               -40~+70
      Temperature
       Humidity                                                5% ~95%
       
Pin Description:

   No            Pin            Description                                Note
    1            VCC           Input Voltage                   DC3.2~5.0V;Type: DC4.2V
    2            RX          UART serial input
    3             TX        UART serial output
    4           DAC_R    Audio output right channel            Drive earphone and amplifier
    5           DAC_L    Audio output left channel             Drive earphone and amplifier
    6            SPK2            Speaker                       Drive speaker less than 3W
    7            GND              Ground                               Power GND
    8            SPK1            Speaker                      Drive speaker less than 3W
                                                          Short press to play previous（ long press
    9            IO1           Trigger port 1
                                                                   to decrease volume）
    10           GND              Ground                               Power GND
                                                          Short press to play next（long press to
    11           IO2           Trigger port 2
                                                                    increase volume）
    12          ADKEY1           AD Port 1                      Trigger play first segment
    13          ADKEY2           AD Port 2                      Trigger play fifth segment
    14           USB+            USB+ DP                                 USB Port
    15           USB-            USB- DM                                 USB Port
    16           BUSY          Playing Status              Low means playing \High means no
    
    The DF Player will receive the command through serial communication protocol. 
    The command has to be in specific format to play/Pause/Cancel music
    
    For example, select the first song played, serial transmission command is: 7E FF 06 03 00 00 01 FF E6 EF
    
      7E --- START command
      FF --- Version Information
      06 --- Data length (not including parity)
      03 --- Representative No.
      00 --- If need to acknowledge [0x01: need answering, 0x00: do not need to return the response]
      00 --- Tracks high byte [DH]
      01 --- Tracks low byte [DL], represented here is the first song played
      FF --- Checksum high byte
      E6 --- Checksum low byte
      EF --- End Command
    
    
    Refer the manual ![Manual](/DFPlayer Mini Manual.pdf "Manual") for more information and examples
    
    
    

  
