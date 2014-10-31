#pragma once
#include "StdAfx.h"



using namespace std;
struct JSONPair
{
public:	
	string key;
	Object value;

	static JSONPair decode(char* _str, int len);	
};
