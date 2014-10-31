
#pragma once
#include "stdafx.h"
#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(void)
{
}
ByteBuffer::ByteBuffer(byte* buffer)
{
	this->_buff = buffer;
}
ByteBuffer::ByteBuffer(byte* buffer, int cap)
{
	try 
	{	
		this->_capacity = cap;	
		this->_position = 0;
		this->_buff = buffer;
	} 
	catch (int e) 
	{
		_buff = null;
	}
}

ByteBuffer::ByteBuffer(byte* buffer, int dstStart, int lenght)
{
	try {
	LPVOID lp = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(byte)*(lenght - dstStart));

	this->_buff = (byte*)lp;//new byte[lenght - dstStart];
	
	this->_capacity = lenght;
	this->_clear();
	this->_position = 0;
	//this->_buff = &buffer[start];	
	int i;
	int n;
	for (i = dstStart, n = 0; i < dstStart + (lenght); i++, n++) {
		_buff[n] = buffer[i];
	}	

	} catch (int e) {
		_buff = null;
	}
}

ByteBuffer::ByteBuffer(int cap)
{
	try {

	this->_buff = (byte*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(byte)*cap);//new byte[cap];
	this->_capacity = cap;
	//this->_clear();
	this->_position = 0;
	} catch (int e) {
		_buff = null;
	}
}



ByteBuffer::~ByteBuffer(void)
{
	HeapFree(GetProcessHeap(), NULL, (LPVOID)_buff);
	//delete[] _buff;

}
void ByteBuffer::_clear(void)
{
	_clear(_buff, _capacity);
}
void ByteBuffer::_clear(byte* b, int cap)
{
	if (b != 0) {
		for (int i = 0; i < cap; i++) {
			b[i] = 0;
		}
	}
}
/*
ByteBuffer* ByteBuffer::wrap(byte* arr, int size)
{
	return ByteBuffer::wrap(arr, 0, size);	
}
ByteBuffer* ByteBuffer::wrap(byte* arr, int start, int count)
{
	byte* buffer = new byte[count - start];
	if (buffer == 0) { return 0; }
	for (int i = start; i < count; i++) {
		buffer[i] = arr[i];
	}	
	return new ByteBuffer(buffer, (count - start));
}
ByteBuffer* ByteBuffer::allocate(int capacity) 
{
	
	try {
		byte* buffer = new byte[capacity];
		_clear(buffer, capacity);
	} catch (int e) {
		return null;
	}
	
	try {		
		ByteBuffer* bbuf = new ByteBuffer(capacity);
		return bbuf;
	} catch (int e) {
		return null;
	}
	
}
*/
void ByteBuffer::put(byte b)
{
	put(b, _position);
}
// Base put method
void ByteBuffer::put(byte b, int pos)
{
	if (_buff == 0) { return; }
	if (pos <= _capacity) {
		_buff[pos] = b;
		//*_buff += b;
		_position++;
	}
}
void ByteBuffer::put(byte* b, int start, int count)
{
	int i;
	int n;
	if (_buff != null) {		
		for (i=0, start; ((i<count)&&(start<_capacity)); i++, start++)
		{
			//_buff[start] = b[i]; //->getByte(i);
			put(b[i], start);
		}
	}
	//_position = start;
}
void ByteBuffer::put(byte* b, int count)
{
	put(b, _position, count);
}

void ByteBuffer::put(ByteBuffer* buf, int pos)
{	
	byte* b;
	b = buf->getArray();
	int cc;
	cc = buf->getCapacity();
	put(b, _position, cc);
}

void ByteBuffer::put(ByteBuffer *nbuf)
{
	put(nbuf, _position);
}
void ByteBuffer::get(byte bytes[], int startWriteAt, int startReadAt, int count)
{
	int i, n;
	for (i = startReadAt, n = startWriteAt; i < startReadAt + count; i++, n++)
	{
		bytes[n] = _buff[i];
	}
	_position = i;
}
void ByteBuffer::get(byte bytes[], int count)
{
	get(bytes, 0, _position, count);
}



void ByteBuffer::putFloat(float f)
{
	putFloat(f, _position);
}
void ByteBuffer::putFloat(float f, int pos)
{
	//char *buff = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 20);
	//byte *temp = (byte*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 4);	
	if (_buff == 0) { return; }
	byte temp[4];
	int i = 0, n = 0;
	*((float*)temp) = f;
	if (pos <= _capacity) {		
		//*(((float*)_buff) + pos) = f;
		for (i = pos, n = 0; (i < pos + 4); i++, n++)
		{			
			put(temp[n], i);
		}				
	}
	//HeapFree(GetProcessHeap(), NULL, buff);
}
byte ByteBuffer::getByte()
{
	return getByte(_position);
}
byte ByteBuffer::getByte(int pos)
{	
	if (_buff == 0) { return 0; }
	byte b = 0;
	if (pos <= _capacity) {
		b = _buff[pos];
		_position += 1;		
	}
	return b;
}

float ByteBuffer::getFloat()
{
	return getFloat(_position);
}

float ByteBuffer::getFloat(int pos)
{
	
	if (pos <= _capacity) {		
		float f;
		int i,n;
		byte temp[4];
		for (i = pos, n = 0; (i < pos + 4); i++, n++)
		{
			temp[n] = getByte(i); 
		}
		f = *(float*)temp;		
		//HeapFree(GetProcessHeap(), NULL, buff);
		return f;
	}	
	return 0;	
}

void ByteBuffer::putChar(char c)
{	
	putChar(c, _position);	
}
void ByteBuffer::putChar(char c, int pos)
{
	if (pos <= _capacity) {
		//_buff[pos] = c;
		put(byte(c), pos);
		//_position++;
	}
}
char ByteBuffer::getChar()
{	
	return getChar(_position);

}
char ByteBuffer::getChar(int pos)
{
	char c = 0;
	if (pos <= _capacity) {
		c = (char)getByte(pos);
		//c = *(_buff + pos);
		//_position += 1;
	}
	return c;	
}

void ByteBuffer::putString(char *str, int pos)
{
	if (_buff == null) { return; }
	int i,n;
	for (i = 0, pos; ((str[i] != 0) && (pos <= _capacity)); i++, pos++)
	{
		putChar(str[i], pos);
	}
	//_position = n;
}
char* ByteBuffer::getString(int pos)
{
	if (_buff == null) { return null; }
	char temp[100];
	int i,n;
	for (i = 0, pos; ((_buff[pos] != 0) && (pos <= _capacity)); i++, pos++)
	{
		//putChar(str[i], pos);
		temp[i] = getChar(pos);
	}
	char* text = new char[i+1];
	text = temp;
	text[i] = 0;
	return text;
}
char* ByteBuffer::getString()
{
	return getString(_position);
}
void ByteBuffer::putString(char* str) 
{
	putString(str, _position);
}