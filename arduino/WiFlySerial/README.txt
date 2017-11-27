WiFly Driver for the Arduino platform
Provides WiFi wireless communications for Arduino-based systems.
Arduino 1.0 platform

============
TRADEMARKS
============
"WiFly", "RN-131","RN-171", "RN-174", "RN-XV" are trademarks / tradenames of Roving Networks Inc.
"Arduino" is a trademark of Arduino LLC.
Other trademarks referenced are the property of their respective owners.

============
LICENSE
============

(C) 2011, 2012 Tom Waldock

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

============
INSTALLATION
============

Library version:
Copy the WiFlySerial files and Examples folders to a new folder in your Arduino Libraries path
(e.g. /usr/share/arduino/libraries/WiFlySerial)
Restart the IDE; the examples and libraries should then become available.
Also: install Streaming.h, PString.h from arduiana.org, and Time.h from arduino.cc/libraries


WebTimeA1.ino is a simple example webserver that shows the current time.
Aim your browser at the Arduino+WiFly's IP address for the current UTC time.
Add "/status" to the URL for additional status items, or "/auto" for 30-second refreshes.

WiFly_TestA1.ino is a simple terminal for communicating to and from the WiFly.
It is useful for exploring its features and debugging issues.

WFSv3 is a proof-of-concept demo of Client/Server libraries based on the Arduino EthernetServer and EthernetClient models.

=======
SUPPORT
=======

Current code and support forums are available from http://arduinowifly.sourceforge.net.
Additional commentary on http://arduinology.blogspot.com and http://arduinology.tumblr.com


=======
HISTORY
=======
*** 2012-May-05 1.08 Minor fix to setRemotePort();
*** 2012-Apr-22 1.07 Time sync handling improvements and Client/Server library compilation issue resolved.

*** 2012-Apr-14 1.06 Clean closing and faster data->command mode switching.
    Preview of Client/Server libraries

*** 2012-Mar-22 1.05 Noise reduction
   Miscellaneous stabilizations, cleanups and now finishes sending before listening.

*** 2012-Feb-25 1.03 - Arduino 1.0 compatible (as are future versions)
   Implemented serial functions available(), peek(), read(), flush(), write() to replace direct uart calls.
   read() now watches for in-stream command signals especially *CLOS*.
   uart will become private in an upcoming release.

*** 2012-Feb-24 1.02 beta released - Arduino 1.0 compatible
   closeCommand() now avoids calling close command if TCP connection already closed.
   WebClientGetPost returned to Examples.

*** 2012-Feb-22 1.01 beta released - Arduino 1.0 compatible.
   Buffer-overflow condition removed from core routines - stability improved.
   Debug methods altered to streaming form.
   Replaced hard-coded timing values with defines.

*** 2012-Feb-18 Arduino 1.0-compatible version released.

Arduino 1.0 compatibility changes
   NewSoftSerial replaced by SoftwareSerial.
   F() available, although GetBuffer_P() continues in legacy usage.
   Time.h to current version.
   PString.h to current version.
   Streaming.h to current version.

Memory handling changes, reduced footprint
   Methods now create (and release) their own buffers as needed, 
    no longer borrowing & over-writing provided buffers.
   Strings moved to static (program) space
   Some functions using only 20-byte command buffers instead of 64-byte or larger.

Added status methods
   IsConnected renamed to isifUp()
   getDeviceStatus() retrieves a status byte interpretable with is...() methods.  
   Reduces impact on WiFly during status interrogations.
   See isAssociated() isTCPConnected(), isAuthenticated(), isDNSfound() et al.
   isInCommandMode() and isConnectionOpen() exposed and internal flags made private.
   ScanForPattern watches for *OPEN* and *CLOSED* signals.  
     *** once streaming enabled, the uart will also catch these.

Naming Changes
   Method names are now more consistently camel-case.  Some sketches may break and require updates.  Sorry.

Added server, port, UDP methods
   New methods added to better support server mode, port definitions, and protocol (TCP/UDP) changeover.
   Simplifies waiting for connections, setting of ports and protocols.

Added WiFi configuration methods
   New methods for configuring Ad-hoc/Infrastructure modes, authentication modes, and DHCP options.

Utility Functions
   New methods provided for conversion between IP addresses represented in strings and byte arrays.
   
Bugs:
   Some fixed.
   Others added.  Please see support forums for bug topics!

Known issues:
   1. With WiFly 2.32 occasional difficulties getting Arduino - WiFly communications to solidify within first attempts... Sometimes it cycles a few times prior to settling down.  Comments and experiences welcome.
   2. closeConnection() does so successfully but WiFly also reports an error.
   3. Keep > 150 bytes free RAM for WiFlySerial buffer allocation/release operations.
   4. Keep baud speeds down to 9600 for now.  Faster may well be possible.
   





*** 2011-Apr-07 Removed bug in ExtractDetail introduced yesterday.

*** Initial library release.

2011-Apr-06 Added status methods, corrected memory handling, re-packaged for library.

*** Initial release.
