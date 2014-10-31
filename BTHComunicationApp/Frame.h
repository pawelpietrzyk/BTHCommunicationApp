#pragma once
#include "stdafx.h"
#define START_BYTE_VALUE	66
#define END_BYTE_VALUE		69




class Frame
{
public:
	Frame(void);
	Frame(byte* buff, int buffcc);
	Frame(ClsObject* objs, char* format);
	~Frame(void);
	ByteBuffer* codeFrame(ClsObject objs[], char* f);
	ClsObject* decodeFrame(ByteBuffer* buff, int *count);
	int initToSendFrame(ClsObject objs[], char* frm);
	int initReceivedFrame(byte* buffer, int size);
	ByteBuffer* getBytes() { return bytes; };
	int getObjects(ClsObject* arr) {
		arr = objs;
		return objCount;
	};
	void setObjects(ClsObject* arr, int count) {
		objs = arr;
		objCount = count;
	};
	void setFormat(char* frm) {
		format = frm;
	};
	char* getFormat() {
		return format;
	};
private:	
	int bcc;
	ByteBuffer* bytes;
	ClsObject* objs;
	char* format;
	int objCount;
	bool isReady;
	int dest;

	int getCapacity(char* frm);
	int getLenght(char* frm);
	
	


	/*
	unsigned __int8 start;
	unsigned __int8 end;
	unsigned __int8 type;
	unsigned __int8 variables_qty;
	unsigned __int8 variable_size;
	*/
};
