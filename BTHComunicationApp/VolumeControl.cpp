#pragma once
#include "StdAfx.h"
#include "VolumeControl.h"
#include "JSONObject.h"
#include "JSONPair.h"


VolumeControl::VolumeControl(void)
{
}

VolumeControl::~VolumeControl(void)
{
}

void VolumeControl::setPair(JSONPair _pair)
{
	string key;
	Object value;

	key = _pair.key;
	value = _pair.value;
	if (key == "type")
	{
		string type = value.toString();
		if (type == "Action")
		{
			this->state = State::Action;
		}
		if (type == "Init")
		{
			this->state = State::Initialize;
		}
		if (type == "Msg")
		{
			this->state = State::Message;
		}

	}

	if (key == "action")
	{
		string sValue = value.toString();

		if (sValue == "Up")
		{
			this->volumeAction = ActionType::Up;
		}
		if (sValue == "Down")
		{
			this->volumeAction = ActionType::Down;
		}
		if (sValue == "Mute")
		{
			this->volumeAction = ActionType::Mute;
		}
		if (sValue == "Exit")
		{
			this->volumeAction = ActionType::Exit;
		}			
	}

	if (key == "max")
	{
		this->volumeMax = value.toInt();
	}
	if (key == "min")
	{
		this->volumeMin = value.toInt();
	}
	if (key == "mute")
	{
		this->volumeMute = value.toBool();
	}
	if (key == "level")
	{
		this->volumeLevel = value.toInt();
	}
}

VolumeControl VolumeControl::unpack(char* s, int len)
{
	VolumeControl vc;
	char begin = '{';	
	char end = '}';
	char terminator = ',';
	char* mPair = new char[255];
	int iPair = 0;
	bool isBegan;
	bool isEnded;
	JSONPair jpair;
	int i = 0;	
	int iJson = 0;
			
	for (i; i < len; i++)
	{
		if (s[i] == begin)
		{
			isBegan = true;
			continue;
		}		
		if (isBegan)
		{			
			if ((s[i] == terminator) || (s[i] == end))
			{
				jpair = JSONPair::decode(mPair, iPair);
				vc.setPair(jpair);				
				iJson++;
				delete mPair;
				mPair = new char[255];
				iPair = 0;
				continue;			
			}
			mPair[iPair] = s[i];
			iPair++;
		}
			
		if (s[i] == end)
		{
			//jpair = JSONPair::decode(mPair, iPair);
			//vc.setPair(jpair);
			isEnded = true;
		}
	}
	
	return vc;
}

VolumeControl* VolumeControl::createFromFrame(Frame* frm)
{
	VolumeControl *vc = new VolumeControl();
	ClsObject* objs;
	int objsCount;
	byte b1;
	
	if (frm != null)
	{
		objsCount = frm->getObjects(objs);

		if (objs != null)
		{
			byte b = objs[0].toByte();
			switch (b)
			{
			case (byte)TOF::INIT:
				vc->state = State::Initialize;

				break;
			case (byte)TOF::MESSAGE:
				vc->state = State::Message;

				break;
			case (byte)TOF::SIGN:
				vc->state = State::Action;
				b1 = objs[1].toByte();
				if (b1 == UP)
				{
					vc->volumeAction = ActionType::Up;
				}
				if (b1 == DOWN)
				{
					vc->volumeAction = ActionType::Down;
				}
				if (b1 == MUTE)
				{
					vc->volumeAction = ActionType::Mute;
				}
				if (b1 == EXIT)
				{
					vc->volumeAction = ActionType::Exit;
				}				
				break;
			default: break;
			}
		}

	}
	
	return vc;
}

Frame* VolumeControl::pack(TOF _tof, byte _sign)
{
	
	ClsObject* objs;
	char* format;	

	switch (_tof) 
	{
		case INIT:
			format = "BFFF";
			objs = new ClsObject[4]; 
			objs[0].fromByte((byte)((int)TOF::INIT));
			objs[1].fromInt(this->volumeLevel);
			objs[2].fromInt(this->volumeMax);
			objs[3].fromInt(this->volumeMin);
			break;
		case MESSAGE: 
			objs = new ClsObject[5];
			format = "BFFFB";
			objs[0].fromByte((byte)((int)TOF::MESSAGE));
			objs[1].fromInt(this->volumeLevel);
			objs[2].fromInt(this->volumeMax);
			objs[3].fromInt(this->volumeMin);
			objs[4].fromBool(this->volumeMute);
			break;			
		case SIGN: 
			objs = new ClsObject[2];
			format = "BB";
			objs[0].fromByte((byte)((int)TOF::SIGN));
			objs[1].fromByte(_sign);
			break;
		default: break;
	}

	if (objs != null)
	{
		Frame* frm = new Frame(objs, format);
		return frm;
	}
	return null;
}


/*
VcStruct VolumeControl::decode( const Object& obj )
{
    VcStruct stru;

    for( Object::size_type i = 0; i != obj.size(); ++i )
    {
        const Pair& pair = obj[i];

        const string& name  = pair.name_;
        const Value&  value = pair.value_;

        if( name == "action" )
        {
			stru.action = value.get_int();           
        }
        else if( name == "init" )
        {
			stru.type = value.get_str();
        }
        else if( name == "max" )
        {
			stru.max = value.get_int();			
        }
        else if( name == "min" )
        {
			stru.min = value.get_int();            
        }
        else if( name == "level" )
        {
			stru.level = value.get_int();            
        }
        else
        {
            assert( false );
        }
    }

    return addr;
}

*/

