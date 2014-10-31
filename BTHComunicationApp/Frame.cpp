#include "stdafx.h"
#include "Frame.h"

Frame::Frame(void)
{
}
Frame::Frame(byte* buff, int buffcc)
{
	isReady = false;
	dest = FRD::READ;
	if (this->initReceivedFrame(buff, buffcc) == 0) {
		ClsObject* obj = decodeFrame(bytes, &objCount);
		setObjects(obj, objCount);
		isReady = true;
	} else { isReady = false; }
	
}

Frame::Frame(ClsObject *objs, char *format)
{
	isReady = false;
	dest = FRD::WRITE;
	int cc = getLenght(format);
	setObjects(objs, cc);
	if (initToSendFrame(objs, format) == 0) { isReady = true;}
	else { isReady = false; }
	
}

Frame::~Frame(void)
{
	delete objs;
	delete format;
	delete bytes;
}

ByteBuffer* Frame::codeFrame(ClsObject* obj, char* f)
{
	int lenght = getLenght(f);
	int cc = getCapacity(f);
	ByteBuffer* buf = new ByteBuffer(cc + lenght);	
	if (buf != 0) {
		buf->put(lenght);
		buf->putString(f);
		for (int i = 0; i <= lenght; i++) {
			//char c = f[i];
			switch (f[i])
			{
			case 'B':				
				buf->put(obj[i].toByte());
				break;
			case 'F':				
				buf->putFloat(obj[i].toFloat());
				break;
			}
		}
	}
	return buf;
}

int Frame::initToSendFrame(ClsObject objs[], char* frm)
{
	if ((objs != 0) && (frm != 0)) {
		ByteBuffer* tbuf = codeFrame(objs, frm);
		bytes = new ByteBuffer(tbuf->getCapacity() + 2);
		bytes->put(START_BYTE_VALUE);
		bytes->put(tbuf);
		bytes->put(END_BYTE_VALUE);
		return 0;
	}
	return 1;
}

ClsObject* Frame::decodeFrame(ByteBuffer *buff, int* count)
{
	int cfrm = (int)buff->getByte();
	count = &cfrm;
	byte* bfrm = new byte[cfrm];
	buff->get(bfrm, cfrm);
	setFormat((char*)bfrm);
	ClsObject* data =  new ClsObject[cfrm];
	for (int i=0; i < cfrm; i++)
	{
		switch (bfrm[i])
		{
		case 'B': data[i].fromByte(buff->getByte()); break;
		case 'F': data[i].fromFloat(buff->getFloat()); break;
		}
	}

	return data;
}


int Frame::initReceivedFrame(byte* buffer, int count)
{
	try {
		int bcc = -1;
		int start = -1;
		bool in = false;
		if (buffer != null) 
		{
			for (int i = 0; i < count; i++)
			{
				if (buffer[i] == END_BYTE_VALUE) {
					in = false;
					break;
				}
				if (in) { bcc++; }
				if (buffer[i] == START_BYTE_VALUE) {
					in = true;
					start = i;
					bcc = 0;
				}
			}
			if (start == -1) { return 1; }
			start++;
			bytes = new ByteBuffer(buffer, start, bcc);
			return 0;
		}
	} catch (int e) {
		return 2;
	}
	return 3;
}

int Frame::getCapacity(char* frm) {
	int cc = 0;	
	for (int i = 0; frm[i] != 0; i++) {
		char c = frm[i];
		switch (c)
		{
		case 'B': c += 1; break;
		case 'F': c += 4; break;
		}
	}
	return cc;
}
int Frame::getLenght(char* frm)
{
	int cc;	
	for (cc = 0; frm[cc] != 0; cc++) {		
	}
	return cc;
}
