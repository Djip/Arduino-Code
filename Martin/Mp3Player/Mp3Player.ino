#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Song.h"
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal.h>
#include <TimedAction.h>

//#################-Lightshow settings
const byte rgbLed[] = {2,3,4, 5,6,7}; //order is R,G,B,R,G,B 
//NOTE: standalone rbgled uses RBG (Red Blue Green) NOT RGB (Red Green Blue) so we swap the order here so we don't need to worry about it later
//NOTE: onbord rgb led uses RGB NOT RBG

//#################-MP3 player settings
SoftwareSerial mp3Serial(10, 11); // RX, TX on the arduino that goes to the mp3 player
DFRobotDFPlayerMini mp3Player;
void printDetail(uint8_t type, int value);

bool mp3IsPlayering = false, mp3IsLooping = false;

//#################-Songs
Song songs[3] = 
{
	{
		"Spyro A Heros Tail", //name
		"Coastal Remains", //info / also used for long names
		2, //mins
		56 //secs
	},
	{
		"Rock And Roll",
		"Mcdonalds",
		2,
		22
	},
	{
		"Rayman 2",
		"The Woods Of Light",
		3,
		39
	},
};

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
byte timeCodeMin, timeCodeSec; //used for time code
byte currentSong;
const int LCDUpdateIntaval = 1000; // 1 sec
unsigned long LCDPreviousMillis = 0;
//------------------------------------------------Arduino stuff

void setup()
{
	Serial.begin(4800); //The mp3player use 9600 so we stick to 4800

	initMP3();
	initLCD();
	rgbLedSetup();

	//Startup mp3
	setMp3Volume(0); //defult 26
	loopMp3(false);
	StartNewPlayback(1);
}

void loop()
{ 

	if (mp3Player.available() && Serial) 
	{
		printDetail(mp3Player.readType(), mp3Player.read()); //Print the detail message from DFPlayer to handle different errors and states.
	}

	if (mp3IsPlayering && (unsigned long)(millis() - LCDPreviousMillis) > LCDUpdateIntaval)
	{
		LCDUpdateTime();
		LCDPreviousMillis = millis();
	}

	//lightShow01(350, 60);
	//lightShow02(350, 60);
	//lightShow03(120);
}

//------------------------------------------------Light Code

void rgbLedSetup()
{
	for (int i; i < sizeof(rgbLed); i++)
	{
		pinMode(rgbLed[i], OUTPUT);
	}
}

//controls color for the set of rgbs
void setColor1(byte red, byte green, byte blue)
{
	analogWrite(rgbLed[0], red);
	analogWrite(rgbLed[1], green);
	analogWrite(rgbLed[2], blue);
}

//controls color for the 2ed set of rgbs
void setColor2(byte red, byte green, byte blue)
{
	analogWrite(rgbLed[3], red);
	analogWrite(rgbLed[4], green);
	analogWrite(rgbLed[5], blue);
}

//simple show, uses the first led
void lightShow01(int loopLong, int loopShort)
{
	byte i = 0,j = 0,k = 0;
	unsigned long ledPreviousMillis = 0;
	bool startOnFirstLight = true;
	//use i for all?
	//int loopLong = 350, loopShot = 60; //delay timers

	//blink slow
	while (i <= 3)
	{
		if (startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopLong)
		{
			setColor1(255, 0, 0);
			if (startOnFirstLight)
			{
				startOnFirstLight = false;
				ledPreviousMillis = millis();
			}
		}
		else if (!startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopLong)
		{
			setColor1(0, 0, 255);
			startOnFirstLight = true;
			ledPreviousMillis = millis();
			i += i + 1;
		}

	}

	startOnFirstLight = true;

	//blink fast
	//red
	while (j <= 7)
	{
		if (startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopShort)
		{
			setColor1(255, 0, 0);
			if (startOnFirstLight)
			{
				startOnFirstLight = false;
				ledPreviousMillis = millis();
			}
		}
		else if (!startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopShort)
		{
			setColor1(0, 0, 0);
			startOnFirstLight = true;
			ledPreviousMillis = millis();
			j += j + 1;
		}

	}

	startOnFirstLight = true;

	//blue
	while (k <= 7)
	{
		if (startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopShort)
		{
			setColor1(0, 0, 255);
			if (startOnFirstLight)
			{
				startOnFirstLight = false;
				ledPreviousMillis = millis();
			}
		}
		else if (!startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopShort)
		{
			setColor1(0, 0, 0);
			startOnFirstLight = true;
			ledPreviousMillis = millis();
			k += k + 1;
		}

	}

	//old code for reference
	//for (i; i <= 2; i++)
	//{
	//	setColor1(255,0,0);
	//	delay(loopLong);
	//	setColor1(0,0,255);
	//	delay(loopLong);
	//}

	////fast blink on/off
	////red
	//for(j; j <= 5; j++)
	//{
	//	setColor1(255,0,0);
	//	delay(loopShort);
	//	setColor1(0,0,0);
	//	delay(loopShort);
	//}

	////blue
	//for(k; k <= 5; k++)
	//{
	//	setColor1(0,0,255);
	//	delay(loopShort);
	//	setColor1(0,0,0);
	//	delay(loopShort);
	//}
}

//simple show, use 2ed leds
void lightShow02(int loopLong, int loopShort)
{
	byte i = 0,j = 0,k = 0;
	unsigned long ledPreviousMillis = 0;
	bool startOnFirstLight = true;
	//use i for all?
	//int loopLong = 350, loopShot = 60; //delay timers

	//blink slow
	while (i <= 3)
	{
		if (startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopLong)
		{
			setColor2(255, 0, 0);
			if (startOnFirstLight)
			{
				startOnFirstLight = false;
				ledPreviousMillis = millis();
			}
		}
		else if (!startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopLong)
		{
			setColor2(0, 0, 255);
			startOnFirstLight = true;
			ledPreviousMillis = millis();
			i += i + 1;
		}

	}

	startOnFirstLight = true;

	//blink fast
	//red
	while (j <= 7)
	{
		if (startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopShort)
		{
			setColor2(255, 0, 0);
			if (startOnFirstLight)
			{
				startOnFirstLight = false;
				ledPreviousMillis = millis();
			}
		}
		else if (!startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopShort)
		{
			setColor2(0, 0, 0);
			startOnFirstLight = true;
			ledPreviousMillis = millis();
			j += j + 1;
		}

	}

	startOnFirstLight = true;

	//blue
	while (k <= 7)
	{
		if (startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopShort)
		{
			setColor2(0, 0, 255);
			if (startOnFirstLight)
			{
				startOnFirstLight = false;
				ledPreviousMillis = millis();
			}
		}
		else if (!startOnFirstLight && (unsigned long)(millis() - ledPreviousMillis) >= loopShort)
		{
			setColor2(0, 0, 0);
			startOnFirstLight = true;
			ledPreviousMillis = millis();
			k += k + 1;
		}

	}

	//old code for reference
	//blink slow
	//for (i; i <= 2; i++)
	//{
	//	setColor2(255,0,0);
	//	delay(loopLong);
	//	setColor2(0,0,255);
	//	delay(loopLong);
	//}

	////fast blink on/off
	////red
	//for(j; j <= 5; j++)
	//{
	//	setColor2(255,0,0);
	//	delay(loopShort);
	//	setColor2(0,0,0);
	//	delay(loopShort);
	//}

	////blue
	//for(k; k <= 5; k++)
	//{
	//	setColor2(0,0,255);
	//	delay(loopShort);
	//	setColor2(0,0,0);
	//	delay(loopShort);
	//}
}

//use both leds
void lightShow03(int loopSpeed) // mix first and 2ed led with some rng
{
	byte i = 0; //loop controller
	unsigned long ledPreviousMillis = 0;

	//controls light loop
	enum lightState
	{
		state1,
		state2,
		state3,
	};

	lightState currentState = state1;

	//rng stuff
	randomSeed(analogRead(0));
	byte rng1 = (byte)random(0 , 2);
	randomSeed(analogRead(0));
	byte rng2 = (byte)random(0, 2);
	randomSeed(analogRead(0));
	byte rng3 = (byte)random(0, 2);

	//convet rng to useable rgb values
	//use digital pins insted?
	if (rng1 == 1)
	{
		rng1 = 255;
	}
	if (rng2 == 1)
	{
		rng2 = 255;
	}
	if (rng3 == 1)
	{
		rng3 = 255;
	}
	//make sure not all rng's are 0
	if (rng1 == 0 && rng2 == 0 && rng3 == 0)
	{
		rng1 = 10, rng2 = 255, rng3 = 255;
	}
	//make sure not all rng's are 255
	else if (rng1 == 255 && rng2 == 255 && rng3 == 255)
	{
		rng1 = 255, rng2 = 10, rng3 = 255;
	}

	//Debug
	//Serial.println(rng1);
	//Serial.println(rng2);
	//Serial.println(rng3);
	//Serial.println();

	while (i <= 3)
	{
		if (currentState == state1 && (unsigned long)(millis() - ledPreviousMillis) >= loopSpeed)
		{
			setColor1(rng1, rng2, rng3);
			setColor2(rng2, rng3, rng1);
			if (currentState == state1)
			{
				currentState = state2;
				ledPreviousMillis = millis();
			}
		}
		else if (currentState == state2 && (unsigned long)(millis() - ledPreviousMillis) >= loopSpeed)
		{
			setColor1(rng3, rng1, rng2);
			setColor2(rng1, rng2, rng3);
			if (currentState == state2)
			{
				currentState = state3;
				ledPreviousMillis = millis();
			}
		}
		else if (currentState == state3 && (unsigned long)(millis() - ledPreviousMillis) >= loopSpeed)
		{
			setColor1(rng3, rng1, rng2);
			setColor2(rng1, rng2, rng3);
			currentState = state1;
			ledPreviousMillis = millis();
			i += i + 1;
		}

	}

	setColor1(0,0,0);
	setColor2(0,0,0);

	//for (i; i < 20; i++)
	//{
	//	setColor1(rng1, rng2, rng3);
	//	setColor2(rng2, rng3, rng1);
	//	delay(loopSpeed);
	//	setColor1(rng3, rng1, rng2);
	//	setColor2(rng1, rng2, rng3);
	//	delay(loopSpeed);
	//	setColor1(rng2, rng3, rng1);
	//	setColor2(rng3, rng1, rng2);
	//	delay(loopSpeed);
	//}
}

//led strip?
void lightShow04() // led strip
{}

//------------------------------------------------LCD Code

void initLCD()
{
	// set up the LCD's number of columns and rows:
	//lcd.begin(16, 2); //small LCD
	lcd.begin(20, 4); // big LCD
	// Print a message to the LCD.
	//F() saves SRAM by keeping strings in main program memory only
	lcd.print(F("     LCD Online"));
	lcd.setCursor(0, 1);
	lcd.print(F(" Awaiting input..."));
	lcd.setCursor(0, 2);
	lcd.print(F("   Start up time:"));
	lcd.setCursor(0, 3);
	lcd.print(millis());
}

void LCDUpdateText(byte song)
{
	song = song - 1; //The mp3 player does not accept '0' as a song
	//but since the song data are indexed in a array we need to match that index

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(songs[song].GetSongName());
	lcd.setCursor(0, 1);
	lcd.print(songs[song].GetSongInfo());
	lcd.setCursor(0, 2);
	lcd.print(songs[song].GetSongLength());
}

void resetLCDTimer()
{
	timeCodeMin = 0, timeCodeSec = 0;
}

void LCDUpdateTime()
{
	if (mp3IsLooping)
	{
		lcd.setCursor(0, 3);
		lcd.print(F("Song is looping"));
		//delay(1000); //avoid lcd overload
	}
	else
	{
		//delay(1000); //wait one sec then update timer
		lcd.setCursor(0, 3);
		timeCodeSec++;

		if (timeCodeSec > 59) //60 secs = 1 min
		{
			//So here's a fun bug:
			//just because you reset a var in the program
			//doesn't mean the lcd will update as you expect
			//if the var has two digets ex 22 and set the var to 0
			//the lcd will then display 02 because the lcd only updates the blocks it need
			//not the blocks after it
			lcd.setCursor(3, 3);
			lcd.print(F("               ")); //print a bunch of white space to make the lcd look nice again
			lcd.setCursor(0, 3);

			timeCodeSec = 0;
			timeCodeMin++;
		}

		lcd.print(timeCodeMin), lcd.print(F(":")), lcd.print(timeCodeSec);// print resault as a time code ex '2:56'
	}
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
	  mp3IsPlayering = false; //stop updating the time code
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

void StartNewPlayback(byte songNr)
{
	mp3Player.stop(); //stop current song
	mp3Player.play(songNr);  //Play the given mp3 in the "mp3" folder (etc: 0001 = 1, 0002 = 2)
	currentSong = songNr - 1;

	LCDUpdateText(songNr);
	resetLCDTimer();
	mp3IsPlayering = true;
}

void pauseMp3Player()
{
	mp3Player.pause();
	mp3IsPlayering = false;
}

void unPauseMp3Player()
{
	mp3Player.start();
	mp3IsPlayering = true;
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
		mp3IsLooping = true;
	}
	else
	{
		mp3Player.disableLoop();
		mp3IsLooping = false;
	}
}

//------------------------------------------------Net Code

