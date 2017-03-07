#include "Song.h"
#include <DFRobotDFPlayerMini.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include <LiquidCrystal.h>
#include "Song.h"

//#################-Lightshow settings
byte rgbLed[] = {2,3,4, 5,6,7}; //order is R,G,B,R,G,B NOTE: standalone rbgled use RBG NOT RGB so we swap the order here so we don't need to worry about it later
//NOTE: onbord rgb led uses RGB NOT RBG

//#################-MP3 player settings
SoftwareSerial mp3Serial(10, 11); // RX, TX on the arduino that goes to the mp3 player
DFRobotDFPlayerMini mp3Player;
void printDetail(uint8_t type, int value);

//#################-Songs
Song s[3]; //Test
//#################-LCD
/*
Arduino - LCD
31 - RS
33 - E
35 - D4
37 - D5
39 - D6
41 - D7
*/
//byte LCDPins[] = {31,33,35,37,39,41};
LiquidCrystal lcd(31, 33, 35, 37, 39, 41);
//------------------------------------------------Arduino stuff

void setup()
{
	Serial.begin(4800); //The mp3player use 9600 so we stick to 4800

	initMP3();
	initLCD();
	rgbLedSetup();

	//Startup mp3
	setMp3Volume(15); //defult 26
	loopMp3(true);
	//StartNewPlayback(3);
}

void loop()
{ 

	if (mp3Player.available() && Serial) 
	{
		printDetail(mp3Player.readType(), mp3Player.read()); //Print the detail message from DFPlayer to handle different errors and states.
	}

	//lightShow01(350, 60);
	//lightShow02(350, 60);
}

//------------------------------------------------Light Code

void rgbLedSetup()
{
	for (int i; i < sizeof(rgbLed); i++)
	{
		pinMode(rgbLed[i], OUTPUT);
	}
}

//controls color for the first rgb led
//NOTE: on standalone rgb leds green is weak
void setColor1(byte red, byte green, byte blue)
{
	analogWrite(rgbLed[0], red);
	analogWrite(rgbLed[1], green);
	analogWrite(rgbLed[2], blue);
}

//controls color for the 2ed rgb led
void setColor2(byte red, byte green, byte blue)
{
	analogWrite(rgbLed[3], red);
	analogWrite(rgbLed[4], green);
	analogWrite(rgbLed[5], blue);
}

//simple show, uses the first led
void lightShow01(byte loopLong, byte loopShort)
{
	int i = 0,j = 0,k = 0;
	//use i for all?
	//int loopLong = 350, loopShot = 60; //delay timers

	//blink slow
	for (i; i <= 2; i++)
	{
		setColor1(255,0,0);
		delay(loopLong);
		setColor1(0,0,255);
		delay(loopLong);
	}

	//fast blink on/off
	//red
	for(j; j <= 5; j++)
	{
		setColor1(255,0,0);
		delay(loopShort);
		setColor1(0,0,0);
		delay(loopShort);
	}

	//blue
	for(k; k <= 5; k++)
	{
		setColor1(0,0,255);
		delay(loopShort);
		setColor1(0,0,0);
		delay(loopShort);
	}
}

//show off for now
void lightShow02(byte loopLong, byte loopShort) //only for test now use 2ed leds
{
	int i = 0,j = 0,k = 0;
	//int loopLong = 350, loopShot = 60; //delay timers

	//blink slow
	for (i; i <= 2; i++)
	{
		setColor2(255,0,0);
		delay(loopLong);
		setColor2(0,0,255);
		delay(loopLong);
	}

	//fast blink on/off
	//red
	for(j; j <= 5; j++)
	{
		setColor2(255,0,0);
		delay(loopShort);
		setColor2(0,0,0);
		delay(loopShort);
	}

	//blue
	for(k; k <= 5; k++)
	{
		setColor2(0,0,255);
		delay(loopShort);
		setColor2(0,0,0);
		delay(loopShort);
	}
}

//use both leds
void lightShow03() // mix first and 2ed led
{}

//lde strip?
void lightShow04() // led strip
{}

//------------------------------------------------LCD Code

void initLCD()
{
	// set up the LCD's number of columns and rows:
	//lcd.begin(16, 2); //small LCD
	lcd.begin(20, 4); // big LCD
	// Print a message to the LCD.
	//F() saves sram by keeping strings in main memory only
	lcd.print(F("     LCD Online"));
	lcd.setCursor(0, 1);
	lcd.print(F(" Awaiting input..."));
	lcd.setCursor(0, 2);
	lcd.print(F("   Start up time:"));
	lcd.setCursor(0, 3);
	lcd.print(millis());
}

void LCDUpdateText(String text)
{

}

void LCDUpdateTime(byte time)
{
	lcd.setCursor(0, 3);
}

//------------------------------------------------MP3 Code

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}

void initMP3 ()
{
	mp3Serial.begin(9600); // MUST be 9600

	if (Serial)
	{
		Serial.println();
		Serial.println(F("DFRobot DFPlayer Mini Demo"));
		Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

		if (!mp3Player.begin(mp3Serial)) 
		{  //Use softwareSerial to communicate with mp3.
			Serial.println(F("Unable to begin:"));
			Serial.println(F("1.Please recheck the connection!"));
			Serial.println(F("2.Please insert the SD card!"));
			while(true);
		}
		Serial.println(F("DFPlayer Mini online."));
	}

	mp3Player.EQ(DFPLAYER_EQ_NORMAL); //DFPLAYER_EQ_NORMAL DFPLAYER_EQ_POP DFPLAYER_EQ_ROCK DFPLAYER_EQ_JAZZ DFPLAYER_EQ_CLASSIC DFPLAYER_EQ_BASS
}

//TODO: review
void StartNewPlayback( byte songNr) //return songNr for use LCD?
{
	mp3Player.stop(); //stop current song
	mp3Player.play(songNr);  //Play the given mp3 in the "mp3" folder (etc: 0001 = 1, 0002 = 2)
}

void setMp3Volume(byte vol)
{
	mp3Player.volume(vol);  // the volume to play at (0-30) default 23, higer values use more power!
}

void loopMp3(bool loop)
{
	if (loop)
	{
		mp3Player.enableLoop(); //loop the playing mp3 file
	}
	else
	{
		mp3Player.disableLoop();
	}
}

void initSongs()
{
	s[0].SetSongName("Spyro A Heros Tail");
	s[0].SetSongInfo("Coastal Remains");
	s[0].SetSongLengthMinutes(2);
	s[0].SetSongLengthSeconds(56);

	s[1].SetSongName("Rock And Roll");
	s[1].SetSongInfo("Mcdonalds");
	s[1].SetSongLengthMinutes(2);
	s[1].SetSongLengthSeconds(22);

	s[2].SetSongName("Rayman 2");
	s[2].SetSongInfo("The Woods Of Light");
	s[2].SetSongLengthMinutes(3);
	s[2].SetSongLengthSeconds(39);
}
//------------------------------------------------Net Code
