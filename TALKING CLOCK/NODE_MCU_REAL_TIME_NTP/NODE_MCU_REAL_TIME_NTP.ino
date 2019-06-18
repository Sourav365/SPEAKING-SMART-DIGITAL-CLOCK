
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(D8,D7,D6,D5,D4,D3);

#ifndef STASSID
#define STASSID "sourav"
#define STAPSK  "12345678"
#endif

const char * ssid = STASSID; 
const char * pass = STAPSK; 

unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServerIP; 
const char* ntpServerName = "in.pool.ntp.org";////////////////////CAN CHANGE THE SERVER//////////////////////////////

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print(" NOW TIME IS : ");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
   // Serial.print('.');
    delay(500);
  }
  
  udp.begin(localPort);
}

void loop() {
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    //Serial.println("no packet yet");
  }
  else {
    //Serial.print("packet received, length=");
    //Serial.println(cb);
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    
    /////////////// this is NTP time (seconds since Jan 1 1900)://///////////////////////////////////////////////////////
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    
    ////////////// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:.........70 years in sec. = 2208988800///////
    const unsigned long seventyYears = 2208988800UL;
    /////////////Unix time (GMT) = NTP time - (1970-1900)/////////////////////////////////////////////////////////////////////////
    unsigned long epoch = secsSince1900 - seventyYears + 19800;///////19800 = 5 hrs. + 30 mins.//////FOR INDIA TIME = GMT+05:30:00
    /// UTC is the time at Greenwich Meridian (GMT)



    
//.............................................................................................................................
     Serial.print(epoch);////////////UTC + 5:30:00/////////////////SEND TIME AS SECOND/////////////////////////////////////
     
//..................HOUR...................................................................................................
    //Serial.print((epoch  % 86400L) / 3600); ///////////HOUR = (UTC % 1 DAY) / 1 HOUR........1 DAY = 86400 SEC.
    int hr = (epoch  % 86400L) / 3600 ;
    if (hr <10){
        lcd.setCursor(3,1);
        lcd.print('0');
        lcd.setCursor(4,1);
        lcd.print(hr);
    }
    else {
        lcd.setCursor(3,1);
        lcd.print(hr); 
    }
    
   // Serial.print(':');
    lcd.setCursor(5,1);
    lcd.print(':');
    
//.................MINUTE...................................................................................................
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      //Serial.print('0');
      lcd.setCursor(6,1);
      lcd.print('0');
      lcd.setCursor(7,1);
      lcd.print((epoch  % 3600) / 60);////////////////MIN = (UTC % 1 HOUR) / 1 MIN...........1 Hr. = 3600 Sec.
    }
    else {
    //Serial.print((epoch  % 3600) / 60); 
    lcd.setCursor(6,1);
    lcd.print((epoch  % 3600) / 60);
    }
    
    //Serial.print(':');
    lcd.setCursor(8,1);
    lcd.print(':');
    
//..................SECOND.............................................................................................
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      //Serial.print('0');
      lcd.setCursor(9,1);
      lcd.print('0');
      lcd.setCursor(10,1);
      lcd.print(epoch % 60);
    }
    else {
      //Serial.println(epoch % 60); /////////SEC = UTC % 1 SEC.
      lcd.setCursor(9,1);
      lcd.print(epoch % 60);
    }
  }
  //......................................................................................................................

  delay(29000);
}




// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address) {
  //Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
