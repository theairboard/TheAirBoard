The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Installing libraries:
- Place /TheAirBoard folder in your Preferences>Sketchbook location /libraries folder.
- You may need to create the /libraries subfolder if it is your first library.
- Restart the Arduino IDE.

Compiling:
- Select Tools>Board>Arduino Fio in the Arduino IDE.

Programming via Bluetooth or XBee:
- With Bluetooth, first pair your-device
- Select Tools>Serial Port>your-device in the Arduino IDE.

Programming via WiFi:
- Disconnect from any wired or wireles networks.
- Connect to the adhoc WiFi WiFly/RN-171 network.
- Compile and locate your .hex file.
- Edit and update the WiFi upload script.scpt with your .hex file path and name.
- Launch the WiFi upload script.scpt AppleScript to upload the .hex file.

Check license.txt for more information
All text above must be included in any redistribution