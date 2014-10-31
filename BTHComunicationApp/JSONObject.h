#pragma once
#include "stdafx.h"
#include "JSONPair.h"
#include <list>
//#include <map>
//#include <utility>
//#include <string>

//using namespace std;

struct JSONObject
{
public:
	JSONObject(void);
	
	list<JSONPair> pairList;
	//map<string, Object> pairMap;		
	static JSONObject* deserialize(char* s, int len);

};
