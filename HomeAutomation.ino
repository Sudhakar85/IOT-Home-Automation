#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

#define NEXT 1;
#define PREV 2;
#define STOP 3;
#define PLAY 4;
#define VOLUMNUP 5;
#define VOLUMNDOWN 6;

const char* ssid = "U&ME";
const char* password = "XXXX";
String publicIP = "";
String cloudURI = "/XXX/XXX/data/MYIP_ADDR";

IPAddress ip(192, 168, 1, 100);  // Desired IP Address
IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
WiFiServer server(80);
WiFiClient publicIpClient;
WiFiClient cloudClient;
char IpaddressServer[] = "checkip.dyndns.org";
char cloudServer[] = "api.backendless.com";
SoftwareSerial mySerial(D8, D7);  // RX and TX

// D8 to TX PIN in Player
// D7 to RX PIN in Player

int volumn = 20;

void setup() {

  //Serial.begin(115200);  
  //delay(10);
  //Serial.println("Starting");

  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet); 

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);    
  }
    
  server.begin();
  // Get the Public Ip from this service http://checkip.dyndns.org/ and api.ipify.org
  UpdatePublicIP();

  mySerial.begin(9600);
  playFirst();  
  //Serial.println("Started");
  
  
  
}

void loop() {

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  while (client.connected()) 
  {
    if(client.available())
     {
      
      // Read the first line of the request
      String request = client.readStringUntil('\r');
      //Serial.println(request);
      client.flush();
    
      WriteHTML(client);
      client.stop();
      //Serial.println("client Disconnected");
    
      int control = PLAY;
      if (request.indexOf("/CONTROL=NEXT") != -1) 
      {
        //Serial.println("NEXT");
        playNext();
      } 
      else if (request.indexOf("/CONTROL=PREV") != -1){
        //Serial.println("PREV");
        playPrevious();
      }
      else if (request.indexOf("/CONTROL=STOP") != -1){
        //Serial.println("STOP");
        pause();
      }
      else if (request.indexOf("/CONTROL=PLAY") != -1){
        //Serial.println("PLAY");
        play();
      }
      else if (request.indexOf("/CONTROL=DOWN") != -1){
        //Serial.println("DOWN");
    
        volumn = volumn - 1;
        if(volumn <= 0)
          volumn = 0;
          
        setVolume(volumn);
      }
      else if (request.indexOf("/CONTROL=UP") != -1){
        //Serial.println("UP");
        volumn = volumn + 1;
    
        if(volumn >= 30)
          volumn = 30;
          
        setVolume(volumn);
      }

      break;
     }
   }
   
}


void WriteHTML(WiFiClient client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Content-Length: 500");
  client.println("Connection: close");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html><head></head><body>");
  client.println("<h1>Sudhakar Home<h1>");  
  client.println("<br><br>");
  client.println("<h1>Remote MP3 Player <h1>");
  client.println("<br>");
  client.println("<a href=\"/CONTROL=NEXT\">NEXT</a>");
  client.println("<a href=\"/CONTROL=PREV\">PREVIOUS</a>");
  client.println("<a href=\"/CONTROL=STOP\">STOP</a>");
  client.println("<a href=\"/CONTROL=PLAY\">PLAY</a>");
  client.println("<a href=\"/CONTROL=UP\">VOLUMN UP</a>");
  client.println("<a href=\"/CONTROL=DOWN\">VOLUMN DOWN</a>");
  
  client.println("<br><br>");
  client.println("</body></html>");
  
}
void UpdatePublicIP()
{
  if(publicIpClient.connect(IpaddressServer,80))
  {
       publicIpClient.println("GET HTTP/1.1");
       publicIpClient.println("Host: checkip.dyndns.org");
       publicIpClient.println("Connection: close");
       publicIpClient.println();
       
      while (publicIpClient.available()) 
      {
        String c = publicIpClient.readStringUntil('\r');       
        publicIP = publicIP + c;
      } 
      publicIpClient.flush();
      publicIpClient.stop();
      delay(1);
    if(cloudClient.connect(cloudServer,80))
      {
          publicIP.replace("\n","");
          String content = String("{ \"Addr\" : \"") + publicIP + String(" \" }");          
          String length = String(content.length());
          cloudClient.println("POST "+ cloudURI +" HTTP/1.1");
          cloudClient.println("Host: api.backendless.com");
          cloudClient.println("Accept: */*");
          cloudClient.println("Content-Length: " + length);
          cloudClient.println("Content-Type: application/json");          
          cloudClient.println();
          cloudClient.println(content);
          
          while (cloudClient.available()) 
          {
            // Write the Response from POST
            //Serial.write(cloudClient.read());
          } 
          
          cloudClient.flush();
      }
      publicIpClient.stop();
      cloudClient.stop();
  } 
}

void playFirst()
{
  execute_CMD(0x3F, 0, 0); // Initialize system
  delay(100);
  setVolume(20);
  delay(100);
  execute_CMD(0x11,1,1); 
  //delay(500);
}

void play()
{
  // 0x0D command to play and the folder 00 and the file 001
  execute_CMD(0x0D,0,1); 
  delay(100);
}

void playNext()
{
  execute_CMD(0x01,0,1);
  delay(100);
}

void playPrevious()
{
  execute_CMD(0x02,0,1);
  delay(100);
}

void pause()
{
  execute_CMD(0x0E,0,0);
  delay(100);
}

void setVolume(int volume)
{
  // 0x06 - Command to set volume
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(100);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
 {
  // Calculate the checksum (2 bytes)
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  // Build the command line
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
  Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
  //Send the command line to the module
  for (byte k=0; k<10; k++)
  {
    mySerial.write( Command_line[k]);
  }
  
}
