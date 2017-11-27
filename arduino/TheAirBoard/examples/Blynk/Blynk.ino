/**************************************************************
 *
 * The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
 * ubiquitous computer designed to sketch Internet-of-Things, fast!
 * Visit http://www.theairboard.cc
 * Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
 * Check README.txt and license.txt for more information.
 * All text above must be included in any redistribution.
 *
 ************************************************************** 
 *
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social groups:              http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * This example shows how to use The AirBoard BLE
 * to connect your project to Blynk.
 * Feel free to apply it to any other example. It's simple!
 *
 * 1. plug in the BLE-Link shield to your AirBoard and
 *    the USB BLE-Link dongle to your computer
 *
 * 2. Optional, but recommended.
 *    Connect additional USB-serial adapter to see the prints.
 *
 * 3. Edit auth token and upload this sketch.
 *
 * 4. Run the script (script located in "scripts" folder of library root,
 *    e.g. 'blynk-library/scripts') for redirecting traffic to server:
 *
 *      for Windows:         scripts/blynk-ser.bat
 *      for Linux and OSX:   ./scripts/blynk-ser.sh (may need to run with sudo)
 *
 *    You can specify port, baud rate, and server endpoint like this
 *    (you may skip server parameters when running local):
 *      ./blynk-ser.sh -c <serial port> -b <baud rate> -s <server address> -p <server port>
 *
 *    For instance :
 *      ./blynk-ser.sh -c /dev/tty.usbmodem621 -b 9600 -s cloud.blynk.cc -p 8442
 *
 *    Be sure to select the right serial port (there may be multiple) when prompted:
 *       Select serial port [  ]: <serial_port>
 *
 *    Run blynk-ser.sh -h for more information
 *
 *    Attention!
 *        Arduino IDE may complain with "programmer is not responding".
 *        You need to terminate script before uploading new sketch.
 *
 * 5. To start, drag and drop a single D5 button to control the pin 5 green LED
 *    as shown in this video: https://www.youtube.com/watch?v=cAP668GF3_8
 *
 *    Don't forget to connect your smartphone to your WiFi network
 *
 **************************************************************/

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11);
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  SwSerial.begin(9600);
  Blynk.begin(auth);
  // Default baud rate is 9600. You could specify it like this:
  //Blynk.begin(auth, 57600);
}

void loop()
{
  Blynk.run();
}

