#include <WiFi.h>
#include <WebSocketServer.h>
#include "index.html.h"


/**************************** WIFI ****************************/
const char* ssid     = "";
const char* password = "";


/**************************** motor ***************************/
#define TIMER_WIDTH 8
#define FREQUENCY 50 //HZ
#define MAX_PULSE 8000

#define RIGHT_FWD_PIN 12
#define RIGHT_FWD_CHNL 0

#define RIGHT_REV_PIN 13
#define RIGHT_REV_CHNL 1

#define LEFT_FWD_PIN 14
#define LEFT_FWD_CHNL 2

#define LEFT_REV_PIN 27
#define LEFT_REV_CHNL 3

/**************************************************************/

WiFiServer server(80);
WiFiServer wsServer(8080);
WebSocketServer webSocketServer;

String html = String(INDEX_HTML);

void initMotors() {
  // channel 0, right forward
  ledcSetup(RIGHT_FWD_CHNL, FREQUENCY, TIMER_WIDTH); 
  ledcAttachPin(RIGHT_FWD_PIN, RIGHT_FWD_CHNL);
  
  // chanel 1, right reverse
  ledcSetup(RIGHT_REV_CHNL, FREQUENCY, TIMER_WIDTH); 
  ledcAttachPin(RIGHT_REV_PIN, RIGHT_REV_CHNL);
  
  // channel 2, left forward
  ledcSetup(LEFT_FWD_CHNL, FREQUENCY, TIMER_WIDTH); 
  ledcAttachPin(LEFT_FWD_PIN, LEFT_FWD_CHNL);
  
  // channel 3, left reverse
  ledcSetup(LEFT_REV_CHNL, FREQUENCY, TIMER_WIDTH); 
  ledcAttachPin(LEFT_REV_PIN, LEFT_REV_CHNL);
}

void connectingNetwork() {
  delay(100);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin(); 
  wsServer.begin(); 
}
 
void setup() {
 
  Serial.begin(115200);
  connectingNetwork();
  initMotors();
  delay(100);
}
 
void loop() {

  //TODO handle http server function
  WiFiClient client = server.available();   // listen for incoming clients

  if (client.connected()) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    String data;
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:

            // the content of the HTTP response follows the header:
            client.println();

            //TODO genrate html with ip in setup
            char chtml[html.length()+1];
            html.replace("LOCAL_IP", WiFi.localIP().toString());
            html.toCharArray(chtml, html.length()+1);
            
            client.write(chtml);
            
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }       
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }


//////////////////////
  //TODO WS handle function
  WiFiClient wsClient = wsServer.available();
 
  if (wsClient.connected() && webSocketServer.handshake(wsClient)) {
    String data;      
 
    while (wsClient.connected()) {
      data = webSocketServer.getData();
      if (data.length() > 0) {
         processData(data);
      }
 
      delay(10); // Delay needed for receiving the data correctly
    }

   Serial.println("The ws client disconnected");
   delay(100);
  }
}

void processData(String data) {
  char cData[data.length()+1];
  data.toCharArray(cData, data.length()+1);
  int x = atoi(strtok(cData," "));
  int y = atoi(strtok(NULL," "));
  printf ("X: %d, Y: %d\n",x, y);
  motorControl(x, y);
  ESP_LOGI(TAG, "RAM left %d", esp_get_free_heap_size());
}

void motorControl(int x, int y) {
  x = constrain(x, -100, 100);
  y = constrain(y, -100, 100);

  // handle forward reverse 
  int rightChannel = RIGHT_FWD_CHNL;
  int leftChannel = LEFT_FWD_CHNL;
  if(x < 0) {
    rightChannel = RIGHT_REV_CHNL;
    leftChannel = LEFT_REV_CHNL;   
    x *= -1;
  }

  // handle right left
  float leftPercentage = 1.0;
  float rightPercentage = 1.0;
    if(y>0) {
    rightPercentage = (float)(100.0 - y) / 100.0;
  }
  if(y<0) {
    leftPercentage = (float)(100.0 + y) / 100.0;
  }
  int rightPower = x * rightPercentage;
  int leftPower = x * leftPercentage;
  motorWrite(rightChannel, rightPower);
  motorWrite(leftChannel, leftPower);
  Serial.println("------------------------");
}

void motorWrite(int channel, int dutyPercent) {
  int pulse = 0;
  if(dutyPercent !=0){
    pulse = map(dutyPercent, 0, 100, 0, MAX_PULSE);
  }
  ledcWrite(channel, pulse);
  int otherSideChannel = RIGHT_REV_CHNL;
  if (channel == RIGHT_REV_CHNL) {
    otherSideChannel = RIGHT_FWD_CHNL;
  }
  else if (channel == LEFT_FWD_CHNL) {
    otherSideChannel = LEFT_REV_CHNL;
  }
  else if (channel == LEFT_REV_CHNL) {
    otherSideChannel = LEFT_FWD_CHNL;
  }
  // write to 0 to other side channel hbridge
  ledcWrite(otherSideChannel, 0);
  Serial.print(dutyPercent); Serial.print(" "); Serial.print("Wrote "); Serial.print(pulse); Serial.print(" to motor "); Serial.println(channel);
}

