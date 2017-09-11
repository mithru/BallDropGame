#include <SPI.h>
#include <WiFi101.h>

const char ssid[] = "";       //  your network SSID (name)
const char pass[] = "";   // your network password

WiFiClient client;                // instance of the WIFi client library
int status = WL_IDLE_STATUS;      // WiFi status
IPAddress server(172, 16, 244, 167);    // address of the server

const int notificationLED = 6;

boolean nameChanged = false;

long lastTimeSent = 0;
const int sendInterval = 1000;     // minimum time between messages to the server

String names[] = {"n=M\n", "n=MI\n", "n=MIT\n", "n=MITH\n", "n=MITHR\n", "n=MITHRU\n"};
int namesIndex = 0;

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
  long now = millis();
  if (client.connected()) {
    if (!nameChanged) {
      client.print("i"); // display name
      nameChanged = true;
    }

    if (now - lastTimeSent > 20000) {
      //exit
      client.print("x");
      //client.stop();
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



