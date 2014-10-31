#pragma once
typedef unsigned char byte;


class ByteBuffer
{
	
public:
	ByteBuffer(void);
	ByteBuffer(byte* buffer, int cap);
	ByteBuffer(byte* buffer, int dstStart, int lenght);
	ByteBuffer(byte* buffer);
	ByteBuffer(int capacity);
	~ByteBuffer(void);	
	int getPosition(void) { return this->_position; };
	int getCapacity(void) { return this->_capacity; };
	byte* getArray() { return this->_buff; };
	void put(byte b);
	void put(byte b, int pos);
	void put(byte *b, int start, int count);
	void put(byte *b, int count);
	void put(ByteBuffer* buf, int pod);
	void put(ByteBuffer* buf);
	void get(byte bytes[], int startWirteAt, int startReadAt, int count);
	void get(byte bytes[], int count);
	byte getByte();
	byte getByte(int position);
	void putFloat(float f);
	void putFloat(float f, int pos);
	float getFloat(void);
	float getFloat(int position);
	void putChar(char c);
	void putChar(char c, int pos);
	void putString(char* str, int pos);
	void putString(char* str);
	char* getString(int pos);
	char* getString();
	char getChar(void);
	char getChar(int pos);
	/*
	static ByteBuffer* allocate(int capacity);
	static ByteBuffer* wrap(byte* arr, int size);
	static ByteBuffer* wrap(byte* arr, int start, int count);
	*/
private:
	int _position;
	int _capacity;
	byte* _buff;
	void _clear(void);
	
	void _clear(byte* buf, int cap);
	
};
