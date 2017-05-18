# IOT-Home-Automation
Purpose of the project to Remote control MP3 player in home through internet using ESP8266 Wemos D1

# Things Needed

1. ESP8266 Wemos D1 board
2. Jummper Wire
3. Micro USB Cable
4. Arduino IDE
5. DF Player
6. Micro SD Card
7. 1k Resistor

# 1. ESP8266 Wemos D1 Setup
  Refer my previous Project https://github.com/Sudhakar85/IOT-ESP8266 for Arduino IDE and ESP8266 Setup for programming


# 2. Setup SD Card
Format the SD Card and create the folder name like 01, 11, 31 , 99 etc and upload the MP3 files with name like 001.mp3, 002.m03.
The file name can't be long since file selection is based on the command passed to the DF Player

# 3. Connections
Follow the below PIN diagram and setup the connection. Added 1K resistor to avoid the noise in RX pin

![Diagram](/Diagram.jpg?raw=true "Diagram")

# 4. HOW DF Player Works
    The DF Player will receive the command through serial communication protocol. 
    The command has to be in specific format to play/Pause/Stop music
    
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
    
   
   # 5. Upload Sketch   
   Take HomeAutomation.ino file from this Repostary and modify below code before upload
   
   Change below code to add your WIFI name and password
   
    const char* ssid = "U&ME";
    const char* password = "XXXX";

   I am using https://backendless.com to store my wifi public IP so that i can access the device anywhere using public IP.
   Every time the device starts, it will upload the public IP address in backendless DB. You can remove this logic if you are
   testing the application locally . Below code refer the backendless Key information
   
    String cloudURI = "/XXX/XXX/data/MYIP_ADDR";
   
   Change the device IP Address based on your need
   
    IPAddress ip(192, 168, 1, 100); // Desired IP Address
    
    Once you modified, upload the code into ESP8266 using Arduino IDE
    
    # 5. Program Logic
    
    1. ESP8266 power up
    2. Connect WIFI network
    3. Upload public IP into backendless using REST service POST
    4. Send command to DFplayer to Initialize
       Send command to DFplayer to Play first song    
    5. Take the Public IP and Open the Page in webbrowser
    6. ESP8266 give the webpage with MP3 control links like PLAY, STOP, NEXT, PREV, Volumn UP , Volumn Down
    7. Click any NEXT link
    8. ESP8266 reads the response and send the command to play Next song
    9. Based on the user click in the Link, ESP8266 reads the query string response and send the respective PLAY/STOP/NEXT/PREV
    commands.    
    10. Close the cliend connection 
    
    
