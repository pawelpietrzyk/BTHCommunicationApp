#include "StdAfx.h"
#include "JSONPair.h"



JSONPair JSONPair::decode(char* _str, int len)
{
	char delimiter = ':';
		
	bool isDelimiterHit = false;
	char* mKey = new char[255];
	char* mValue = new char[255];
	int i = 0;
	int iKey = 0;
	int iValue = 0;
	
	for (i; i < len; i++)
	{		
		if (_str[i] == delimiter)
		{
			isDelimiterHit = true;
			continue;
		}
		if (isDelimiterHit)
		{
			mValue[iValue] = _str[i];
			iValue++;
		}
		else
		{
			mKey[iKey] = _str[i];
			iKey++;
		}		
	}
	
	JSONPair pair;
	Object obj;
	mValue += 0;
	mKey += 0;
	obj.parse(mValue, iValue);
	delete mValue;

	String* mStr = new String();
	mStr->parse(mKey, iKey);
	delete mKey;

	pair.key = mStr->str;
	pair.value = obj;
	return pair;

}
