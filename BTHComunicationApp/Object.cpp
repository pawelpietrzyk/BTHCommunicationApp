#pragma once
#include "StdAfx.h"
#include "Object.h"


using namespace std;

ClsObject::ClsObject(void)
{
	
}

ClsObject::~ClsObject(void)
{
}

void ClsObject::parse(char *s, int len)
{
	int type;
	//1 - string
	//2 - int
	//3 - float
	int i = 0;
	char mark = '"';
	char point = '.';
	char* text = new char[len + 1];
	int itext = 0;
	type = 2;
	for (i; i<len; i++)
	{
		if (isdigit((int)s[i]) == 0)
		{
			type = 1;
		}
		if (s[i] == mark)
		{
			type = 1;
			continue;
		}
		
		text[itext] = s[i];
	}
	itext++;
	text[itext] = 0;
	long l;
	float f;
	String* mStr = new String();

	switch (type)
	{
	case 2:
		l = strtol(text, NULL, 0);
		this->fromLong(l);
		break;
	case 3:
		f = atof(text);
		this->fromFloat(f);
		break;
	case 1:
		mStr->parse(text, itext - 1);
		this->fromString(mStr->str);
		break;
	default: break;
	}
		
}

String::String(void)
{

}

void String::parse(char *s, int len)
{
	int i = 0;
	char mark = '"';
	char* text = new char[len + 1];
	
	int itext = -1;

	for (i; i<len; i++)
	{
		if (s[i] == mark)
		{
			continue;
		}
		itext++;
		text[itext] = s[i];
		
	}
	itext++;
	text[itext] = 0;
	this->str = text;
	this->len = itext;
}
Object::Object(void)
{
}
void Object::parse(char* s, int len)
{
	int type;
	//1 - string
	//2 - int
	//3 - float
	int i = 0;
	char mark = '"';
	char point = '.';
	char* text = new char[len + 1];
	int itext = -1;
	type = 2;
	for (i; i<len; i++)
	{
		if (isdigit((int)s[i]) == 0)
		{
			type = 1;
		}
		if (s[i] == mark)
		{
			type = 1;
			continue;
		}
		itext++;
		text[itext] = s[i];
	}
	itext++;
	text[itext] = 0;
	long l;
	float f;
	//String* mStr = new String();
	string mStr;

	switch (type)
	{
	case 2:
		l = strtol(text, NULL, 0);
		this->fromLong(l);
		break;
	case 3:
		f = atof(text);
		this->fromFloat(f);
		break;
	case 1:
		//mStr->parse(text, itext - 1);
		mStr = text;
		this->fromString(text);
		break;
	default: break;
	}
	
}

