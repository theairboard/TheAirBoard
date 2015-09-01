/*
WiFi data logger based on the WiFly RN-XV (RN171) WiFi shield:
- plug the light sensor between the A4 and GND pins
- repeatedly read the light sensor from analog pin A4
- push a notification to http://www.pushingbox.com every minute
- IMPORTANT: WiFly baudrate must first be set to default 9600
- The WiFly module must be in client mode: do not connect GPIO9 to VDD upon power up

1. upload this program to The AirBoard via the BLE-LINK shield
2. replace the BLE-LINK shield by the WiFly RN-XV (RN-171) WiFi shield
3. place the device within your WiFi network coverage area
4. Add an e-mail service on PushingBox and a scenario with $var$ in the message body

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

// Place /TheAirBoard, /Streaming and /WiFlySerial folders in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <TheAirBoard.h>
#include <Streaming.h>
#include <SoftwareSerial.h>
#include <WiFlySerial.h>

#define NETWORK   "Your_SSID_here"
#define PASSWORD  "Your_password_here"
#define DEVICEID  "Your_PushingBox_DeviceID_here"

int data;
WiFlySerial wifi(0,1);                    // 0 = WiFly to The AirBoard, 1 = The AirBoard to WiFly

// the setup routine runs once when you switch on the board:
void setup() {
  pinMode(BLUE, OUTPUT);
  digitalWrite(A4, HIGH);                 // set internal pull-up for light sensor
  wifi.begin();
  wifi.println("set comm open *OPEN*");   // set local UART string on TCP port open
  wifi.println("set comm remote 0");      // set remote TCP client string on TCP port open
  wifi.setPassphrase(PASSWORD);
  wifi.join(NETWORK);
}

// the loop routine runs over and over again forever:
void loop() {  
  digitalWrite(BLUE, 1);                  // set communication indicator
  data = (int)(100.0-100.0*analogRead(A4)/1024); // acquire analog data
  wifi.openConnection("api.pushingbox.com");
  wifi << "GET /pushingbox?devid=" << DEVICEID << "&var=" << data << " HTTP/1.1\nHost: api.pushingbox.com\nUser-Agent: Arduino\n\n";
  wifi.closeConnection();   
  digitalWrite(BLUE, 0);                  // reset communication indicator
  delay(60000);                           // wait for a minute
}


