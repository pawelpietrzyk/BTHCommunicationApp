#pragma once

#include "StdAfx.h"
#include "JSONObject.h"
#include <map>
#include <utility>
#include <iostream>




JSONObject::JSONObject(void)
{
	
}

JSONObject* JSONObject::deserialize(char* s, int len)
{
	
	//map<string, int>::iterator it;
	//pair<map<string, int>::iterator, bool> ret;


	char begin = '{';	
	char end = '}';
	char terminator = ',';
	char* mPair = new char[255];
	int iPair = 0;
	bool isBegan;
	bool isEnded;
	JSONPair jpair;
	int i = 0;
	JSONObject* json = new JSONObject();	
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
			
			if (s[i] == terminator)
			{
				jpair = JSONPair::decode(mPair, iPair);

				//json->pairList.push_back(jpair);	
				//json->put(jpair.key, jpair.value);
				//json->pairMap.insert( pair<string, int>(jpair.key, 2) );

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
			isEnded = true;
		}
	}
	
	return json;


}
