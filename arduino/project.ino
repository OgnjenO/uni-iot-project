#include <Keypad.h>
#include <Ethernet.h>
#define ENCODER_USER_INTERRUPTS
#include <Encoder.h>

char server[] = "alpha.ognjeno.info";
const int serverPort = 5498;
byte mac[] = {0xB4, 0x9E, 0x80, 0x5C, 0x7D, 0x77};
EthernetClient client;

const int ROW_NUM = 4;
const int COLUMN_NUM = 4;
char keys[ROW_NUM][COLUMN_NUM] = {

  {'1','2','3','A'},

  {'4','5','6','B'},

  {'7','8','9','C'},

  {'*','0','#','D'}

};
byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = { 5, 4, A2, A3}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

int VRx = A0;
int VRy = A1;

int xPosition = 0;
int yPosition = 0;
int mapX = 0;
int mapY = 0;
int newMapX = 0;
int newMapY = 0;

#define CLK 2
#define DT 3
Encoder myEnc(CLK, DT);
long oldRotPosition  = 0;

void setup(){
  Serial.begin(9600);

  if(Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtain IP address");

    if(Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield not detected");
    }
    
    if(Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Cable not connected");
    }

    while(true) {
      delay(1000);
    }
  }

  Serial.println(Ethernet.localIP());
  
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
}

 

void loop(){
  char key = keypad.getKey();

  //Serial.println("keypad");
  if (key){
    if(client.connect(server, serverPort)) {
      Serial.println("Connected");
      String target = "GET /";
      target.concat("keypad/");
      target.concat(key);
      target.concat(" HTTP/1.1");
      Serial.print("Sending: ");
      Serial.println(target);
      client.println(target);
      client.println("Host: alpha.ognjeno.info");
      client.println("Connection: close");
      client.println();
      Serial.println("Sent");
    } else {
      Serial.println("Keypad failed");
    }
  }

  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  newMapX = map(xPosition, 0, 1023, -128, 128);
  newMapY = map(yPosition, 0, 1023, 128, -128);
  
  bool shouldUpdateJoystick = false;
  if(abs(mapX-newMapX) > 5) {
    mapX = newMapX;
    shouldUpdateJoystick = true;
  }
  
  if(abs(mapY-newMapY) > 5) {
    mapY = newMapY;
    shouldUpdateJoystick = true;
  }

  if(shouldUpdateJoystick) {
    if(client.connect(server, serverPort)) {
      Serial.println("Connected");
      String target = "GET /";
      target.concat("joystick/");
      target.concat(mapX);
      target.concat("/");
      target.concat(mapY);
      target.concat(" HTTP/1.1");
      Serial.print("Sending: ");
      Serial.println(target);
      client.println(target);
      client.println("Host: alpha.ognjeno.info");
      client.println("Connection: close");
      client.println();
      Serial.println("Sent");
    } else {
      Serial.println("Joystick failed");
    }
  }

  
  long newPosition = myEnc.read();
  if (newPosition != oldRotPosition) {
    oldRotPosition = newPosition;
    if(client.connect(server, serverPort)) {
      Serial.println("Connected");
      String target = "GET /";
      target.concat("rotencoder/");
      target.concat(newPosition);
      target.concat(" HTTP/1.1");
      Serial.print("Sending: ");
      Serial.println(target);
      client.println(target);
      client.println("Host: alpha.ognjeno.info");
      client.println("Connection: close");
      client.println();
      Serial.println("Sent");
    } else {
      Serial.println("Rotencoder failed");
    }
  }
}
