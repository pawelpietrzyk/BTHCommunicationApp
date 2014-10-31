#pragma once


union uobject {
	float _float;
	int _int;
	long _long;
	char _char;
	unsigned char _byte;
	bool _bool;
	char* _string;
} ;
typedef uobject object;

class ClsObject
{
public:
	ClsObject(void);
	ClsObject(object nobj) {
		obj = nobj;
	};
	~ClsObject(void);
	unsigned char toByte() { 
		return obj._byte; 
	};
	bool toBool() { 
		return obj._bool; 
	};
	char toChar() { 
		return obj._char; 
	};
	int	toInt() { 
		return obj._int; 
	};
	float toFloat() { 
		return obj._float; 
	};
	long toLong() { 
		return obj._long; 
	};
	char* toString() {
		return obj._string;
	}
	void fromString(char* s)
	{
		obj._string = s;
	}
	void fromByte(unsigned char b) { 
		obj._byte = b; 
	};	
	void fromBool(bool b) { 
		obj._bool = b; 
	};
	void fromChar(char c) { 
		obj._char = c; 
	};
	void fromInt(int i) {
		obj._int = i; 
	};
	void fromFloat(float f) { 
		obj._float = f; 
	};
	void fromLong(long l) { 
		obj._long = l; 
	};
	void parse(char* s, int len);
private:
	object obj;
};

struct Buffer
{

	char* mBuff;
	int mLenght;
};

class String
{
public:
	String(void);


	char* str;
	int len;

	void parse(char* s, int len);

	static bool isNumber(char c);
};


class Object
{
public:
	Object(void);
	unsigned char toByte() { 
		return obj._byte; 
	};
	bool toBool() { 
		return obj._bool; 
	};
	char toChar() { 
		return obj._char; 
	};
	int	toInt() { 
		return obj._int; 
	};
	float toFloat() { 
		return obj._float; 
	};
	long toLong() { 
		return obj._long; 
	};
	char* toString() {
		return obj._string;
	}
	void fromString(char* s)
	{
		obj._string = s;
	}
	void fromByte(unsigned char b) { 
		obj._byte = b; 
	};	
	void fromBool(bool b) { 
		obj._bool = b; 
	};
	void fromChar(char c) { 
		obj._char = c; 
	};
	void fromInt(int i) {
		obj._int = i; 
	};
	void fromFloat(float f) { 
		obj._float = f; 
	};
	void fromLong(long l) { 
		obj._long = l; 
	};

	void parse(char* s, int len);

private:
	object obj;
};
