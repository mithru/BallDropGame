#include <SPI.h>
#include <WiFi101.h>

const char ssid[] = "";       //  your network SSID (name)
const char pass[] = "?";   // your network password

WiFiClient client;                // instance of the WIFi client library
int status = WL_IDLE_STATUS;      // WiFi status
IPAddress server(172, 22, 151, 123);    // address of the server

const int notificationLED = 6;

boolean nameChanged = false;

long lastTimeSent = 0;
const int sendInterval = 1000;     // minimum time between messages to the server

String names[] = {"n=MTHIRU\n", "n=MITRHU\n", "n=MIRHTU\n", "n=MRITHU\n", "n=MHITRU\n", "n=MITHRU\n"};
int namesIndex = 0;

#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[COLS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[ROWS] = {10, 11, 12, 13}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

boolean active = false;


void setup() {
  pinMode(notificationLED, OUTPUT);

  while ( status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 3 seconds for connection:
    delay(3000);
  }

  for (int i = 0; i < 3; i++) {
    digitalWrite(notificationLED, HIGH);
    delay(100);
    digitalWrite(notificationLED, LOW);
    delay(100);
  }
}


void loop() {

  while (!active) {
    digitalWrite(notificationLED, HIGH);
    delay(100);
    digitalWrite(notificationLED, LOW);
    delay(100);
    char key = keypad.getKey();
    if (key == '5') active = true;
  }

  // keypad stuff
  char key = keypad.getKey();

  // network stuff
  long now = millis();
  if (client.connected()) {
    if (!nameChanged) {
      client.print("i"); // display name
      nameChanged = true;
    }

    //    if (now - lastTimeSent > 20000) {
    //      //exit
    //      client.print("x");
    //      //client.stop();
    //    }

    if (key) {
      switch (key) {
        case '1': client.print("uuuuul");
          break;
        case '2': client.print("uuuuu");
          break;
        case '3': client.print("uuuuur");
          break;
        case '4': client.print("l");
          break;
        case '6': client.print("r");
          break;
        case '7': client.print("dddddl");
          break;
        case '8': client.print("ddddd");
          break;
        case '9': client.print("dddddr");
          break;
        case 'A': client.print("u");
          break;
        case 'D': client.print("ddddddddddddddddddddd");
          break;
        case '*': client.print("x");
          active = false;
          client.stop();
          delay(1000);
          break;
      }
    }

  } // if the client's disconnected, try to connect:
  else {
    nameChanged = false;
    Serial.println("connecting");
    client.connect(server, 8080);
  }


  if (now - lastTimeSent > sendInterval) {
    client.print(names[namesIndex]);
    namesIndex++;
    if (namesIndex > 5)
      namesIndex = 0;
    lastTimeSent = now;
  }

  // if there's incoming data from the client, print it serially:
  if (client.available()) {
    char c = client.read();
    //Serial.write(c);
  }

  // set the connection LED based on the connection state:
  digitalWrite(notificationLED, client.connected());
}


