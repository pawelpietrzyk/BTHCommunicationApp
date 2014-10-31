#pragma once
#include "StdAfx.h"
#include "JSONPair.h"

using namespace std;

#define UNK		0
#define UP		85
#define	DOWN	68
#define	MUTE	77
#define	EXIT	81


enum State 
{
	Initialize,
	Message,
	Action
};

enum ActionType 
{
	Up,
	Down,
	Mute,
	VolumeChange,
	Exit

};


class VolumeControl
{
public:
	VolumeControl(void);
	~VolumeControl(void);

	int setVolume(ClsObject*);
	int setVolumeLevel(int);
	Frame pack(void);
	Frame* pack(TOF _tof, byte _sign);
	void setPair(JSONPair _pair);
	
	int volumeMax;
	int volumeMin;
	int volumeLevel;
	bool volumeMute;
	State state;
	ActionType volumeAction;
	//VcStruct decode( const Object& obj );	

	static VolumeControl* createFromFrame(Frame*);
	static VolumeControl unpack(char* s, int lenght);



};
/*
struct VcStruct
{
	string type;
	string action;
	int action;
	int max;
	int min;
	int level;
	
};

*/

