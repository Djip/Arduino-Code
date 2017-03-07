// 
// 
// 

#include "Song.h"

Song::Song(void)
{
}

Song::Song(String songName, String songInfo, byte mins, byte secs)
{
	SetSongName(songName), SetSongInfo(songInfo), SetSongLengthMinutes(mins), SetSongLengthSeconds(secs);
}

void Song::SetSongName(String name)
{
	_songName = name;
}

void Song::SetSongInfo(String info)
{
	_songInfo = info;
}

void Song::SetSongLengthMinutes(byte length)
{
	_songLengthMinutes = length;
}

void Song::SetSongLengthSeconds(byte length)
{
	_songLengthSeconds = length;
}

String Song::GetSongName(void)
{
	if (_songName == NULL || _songName.length() == 0)
	{
		return "SONG_ERROR NO_NAME";
	}
	else if (_songName.length() > 20)
	{
		return "SONG_ERROR TOO_LONG";
	}

	return _songName;
}

String Song::GetSongInfo(void)
{
	if (_songInfo == NULL || _songInfo.length() == 0)
	{
		return "SONG_ERROR NO_INFO";
	}
	else if (_songInfo.length() > 20)
	{
		return "SONG_ERROR TOO_LONG";
	}

	return _songInfo;
}

String Song::GetSongLength(void)
{
	if (_songLengthMinutes == NULL || _songLengthSeconds == NULL)
	{
		return "SONG_ERROR NO_TIME";
	}
	//we don't need to check to for length here
	//the time code is enterd as a byte mening the max value is 255:255 (7 chars)

	//we don't want the seconds to be more then 59
	if (_songLengthSeconds > 59)
	{
		return "SONG_ERROR INVA_SECS";
	}

	return (String)_songLengthMinutes + ":" + (String)_songLengthSeconds;
}
