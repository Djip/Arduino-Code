// Song.h
#pragma once //fix redefinition of class error
#ifndef _SONG_h
#define _SONG_h

//check if arduino version is higher then 1.00
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Song
{
public:
	Song(void);
	Song(String songName, String songInfo, byte mins, byte secs);
	void SetSongName(String name);
	void SetSongInfo(String info);
	void SetSongLengthMinutes(byte length); //we are NOT gonna have a song longer then 255 minutens.
	void SetSongLengthSeconds(byte length);
	String GetSongName(void);
	String GetSongInfo(void);
	String GetSongLength(void); //retun length like a time code ex. 3:45
private:
	String _songName;
	String _songInfo;
	byte _songLengthMinutes;
	byte _songLengthSeconds;
};
