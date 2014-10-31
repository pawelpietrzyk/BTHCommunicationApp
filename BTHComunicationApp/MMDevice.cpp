#pragma once
#include "stdafx.h"
#include "MMDevice.h"


MMDevice::MMDevice(void)
{
    this->Initialize();
}

MMDevice::~MMDevice(void)
{
    this->Unitialize();
}
void MMDevice::Unitialize(void)
{
    this->masterAudioVolume->UnregisterControlChangeNotify(
        (IAudioEndpointVolumeCallback*)&EPVolEvents);
}

void MMDevice::Initialize()
{
	HRESULT hr;
	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator), 
		NULL, CLSCTX_INPROC_SERVER, 
		__uuidof(IMMDeviceEnumerator), 
		(LPVOID *)&deviceEnumerator);
	if (hr != S_OK) {
		//InsertItem(L"CoCreateInstance failed");
		return;
	}

	IMMDevice *defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;
	if (hr != S_OK) {
		//InsertItem(L"GetDefaultAudioEndpoint failed");
		return;
	}

	//IAudioEndpointVolume *endpointVolume = NULL;
	masterAudioVolume = NULL;
	LPWSTR name;
	defaultDevice->GetId(&name);
	hr = defaultDevice->Activate(
		__uuidof(IAudioEndpointVolume), 
		CLSCTX_INPROC_SERVER, 
		NULL,
		(LPVOID *)&masterAudioVolume);
	defaultDevice->Release();
	defaultDevice = NULL; 	
	if (hr != S_OK) {
		//InsertItem(L"Device Activate failed");
		return;
	}
	hr = masterAudioVolume->RegisterControlChangeNotify(
		(IAudioEndpointVolumeCallback*)&EPVolEvents);
	
	//InsertItem(L"MMDevice Activated:");
	//InsertItem(name);
}

void MMDevice::GetVolumeRange(float *min, float *max, float *step)
{
	HRESULT hr;	
	hr = masterAudioVolume->GetVolumeRange(
		min,
		max,
		step);
}
void MMDevice::GetVolume(float *volLevel)
{
	HRESULT hr;
	//hr = masterAudioVolume->GetMasterVolumeLevel(volLevel);
	hr = masterAudioVolume->GetMasterVolumeLevelScalar(volLevel);
	

}
void MMDevice::SetVolume(float volLevel)
{
	HRESULT hr;
	volLevel /= 100;
	//hr = masterAudioVolume->SetMasterVolumeLevel(volLevel, (LPCGUID)&guid);
	//hr = masterAudioVolume->SetMasterVolumeLevel(volLevel, NULL);	
	hr = masterAudioVolume->SetMasterVolumeLevelScalar(volLevel, NULL);

}
void MMDevice::SetVolumeUP()
{
	HRESULT hr;
	//hr = masterAudioVolume->VolumeStepUp((LPCGUID)&guid);
	hr = masterAudioVolume->VolumeStepUp(NULL);
}
void MMDevice::SetVolumeDOWN()
{
	HRESULT hr;
	//hr = masterAudioVolume->VolumeStepDown((LPCGUID)&guid);
	hr = masterAudioVolume->VolumeStepDown(NULL);
}
void MMDevice::Mute()
{
	HRESULT hr;
	BOOL m = false;	
	hr = masterAudioVolume->GetMute(&m);
	hr = masterAudioVolume->SetMute(!m, (LPCGUID)&guid);
}

float MMDevice::DecodeFrame(char frame[], int frSize)
{
	//int size = GetArrLenght(frame);
	bool inside = false;	
	int incc = 0;
	//string content;
	char content[32];
	float v = 0;
	
	for (int i = 0; i < frSize; i++)
	{
		if (frame[i] == 'E') { 
			inside = false;
			char *tc = new char[incc];
			for (int cc = 0; cc < incc; cc++)
			{
				tc[cc] = content[cc];
			}			
			//DWORD val = atoi(tc);
			v = (float)atof(tc);
			
			//amdSetMasterVolumeValue(val);
			
			//InsertItem(hListView, (LPWSTR)&tc);
			
		}		
		if (inside) {
			if (frame[i] == 'Q') {
				//this->OnQuit();
				return v;
			}
			content[incc] = frame[i];
			incc++;
		}
		if (frame[i] == 'B') { 
			inside = true; 
			incc = 0;
		}
		
	}
	return v;
}

//void MMDevice::CodeFrame(float volume, float min, float max, char frame[], int &osize, char type)
//{
//	int ivol = (int)volume;
//	int imax = (int)max;
//	int imin = (int)min;
//	
//	int ss = sizeof(DWORD);
//	char cmin[33];
//	char cvol[33];
//	char cmax[33];
//	char d = ';';
//	char b = 'B';
//	char e = 'E';
//	_itoa_s(imin, cmin, 33u, 10);	
//	_itoa_s(ivol, cvol, 33u, 10);	
//	_itoa_s(imax, cmax, 33u, 10);
//	
//	size_t sizeCMin = strnlen(cmin, _countof(cmin));
//	size_t sizeCVol = strnlen(cvol, _countof(cvol));
//	size_t sizeCMax = strnlen(cmax, _countof(cmax));
//	unsigned int ibufsize = sizeCMin + sizeCVol + sizeCMax + 2;
//
//	//char buffer[1024] = "";
//	//char frame[1024] = "";
//	
//	int cc = 0;
//	frame[cc] = b;
//	cc++;
//	frame[cc] = type;
//	for (unsigned int i = 0; i < sizeCMin; i++)
//	{
//		cc++;
//		frame[cc] = cmin[i];		
//	}
//	cc++;
//	frame[cc] = d;
//	for (unsigned int i = 0; i < sizeCMax; i++)
//	{
//		cc++;
//		frame[cc] = cmax[i];
//	}
//	cc++;
//	frame[cc] = d;
//	for (unsigned int i = 0; i < sizeCVol; i++)
//	{
//		cc++;
//		frame[cc] = cvol[i];
//	}
//	cc++;
//	frame[cc] = e;
//	//tab = frame;
//	osize = cc;	
//}
void MMDevice::CodeFrame(float volume, float min, float max, char frame[], int &osize, char type)
{
	//int ivol = (int)volume;
	//int imax = (int)max;
	//int imin = (int)min;
	
	char* str = new char[30];
	float flt = 2.4567F;
    
	
	int ss = sizeof(DWORD);
	char* cmin = new char[33];
	char* cvol = new char[33];
	char* cmax = new char[33];
	char d = ';';
	char b = 'B';
	char e = 'E';
	sprintf(cmin, "%.9g", min);
	sprintf(cmax, "%.9g", max);
	sprintf(cvol, "%.9g", volume);
	
	size_t sizeCMin = strnlen(cmin, 33);
	size_t sizeCVol = strnlen(cvol, 33);
	size_t sizeCMax = strnlen(cmax, 33);
	unsigned int ibufsize = sizeCMin + sizeCVol + sizeCMax + 2;

	//char buffer[1024] = "";
	//char frame[1024] = "";
	
	int cc = 0;
	frame[cc] = b;
	cc++;
	frame[cc] = type;
	for (unsigned int i = 0; i < sizeCMin; i++)
	{
		cc++;
		frame[cc] = cmin[i];		
	}
	cc++;
	frame[cc] = d;
	for (unsigned int i = 0; i < sizeCMax; i++)
	{
		cc++;
		frame[cc] = cmax[i];
	}
	cc++;
	frame[cc] = d;
	for (unsigned int i = 0; i < sizeCVol; i++)
	{
		cc++;
		frame[cc] = cvol[i];
	}
	cc++;
	frame[cc] = e;
	//tab = frame;
	osize = cc;	
}

void MMDevice::CodeMsgVolume(float volume, char frame[], int &osize)
{
	int ivol = (int)volume;
	char cvol[33];

	char b = 'B';
	char e = 'E';
	char type = 'M';

	_itoa_s(ivol, cvol, 33u, 10);
	size_t sizeCVol = strnlen(cvol, _countof(cvol));

	int cc = 0;
	frame[cc] = b;
	cc++;
	frame[cc] = type;
	for (unsigned int i = 0; i < sizeCVol; i++)
	{
		cc++;
		frame[cc] = cvol[i];
	}
	cc++;
	frame[cc] = e;
	//tab = frame;
	osize = cc;
}

void MMDevice::CodeMsgMUTE(bool mute, char frame[], int &osize)
{
	//int ivol = (int)volume;
	//char cvol[33];

	char b = 'B';
	char e = 'E';
	char type = 'M';
	char msg[] = "MUTE";
	
	//masterAudioVolume->GetMute(&mute);
	//_itoa_s(ivol, cvol, 33u, 10);
	//size_t sizeCVol = strnlen(cvol, _countof(cvol));
	int size = _countof(msg);
	int cc = 0;
	frame[cc] = b;
	cc++;
	frame[cc] = type;
	for (int i = 0; i < size; i++)
	{
		cc++;
		frame[cc] = msg[i];
	}
	cc++;
	frame[cc] = e;
	//tab = frame;
	osize = cc;
}

/*
void MMDevice::SendMsgVolume(BTHService *writeService)
{
	if (writeService == NULL) { return; }
	float vol;
	float min;
	float max;
	float step;
	char recvbuf[128] = "";
	int fsize = 128;
	//this->GetVolumeRange(&min, &max, &step);
	this->GetVolume(&vol);	
	//vol = ((max - min) / vol);
	vol *= 100;
	this->CodeMsgVolume(vol, recvbuf, fsize);
	int cc = fsize + 1;
	writeService->Write(recvbuf, cc);
}
*/
/*
int MMDevice::SendInitRange(BTHService *writeService)
{
	if (writeService == NULL) { return 1; }
	

	float fmin;
	float fmax;
	float fstep;
	float fval; // = pNotify->fMasterVolume;

	char recvbuf[1024] = "";
	int fsize = 1024;

	this->GetVolumeRange(&fmin, &fmax, &fstep);	
	this->GetVolume(&fval);
	//float n = ((fmax - fmin) / fstep);
	fval *= 100;
	
	this->CodeFrame(fval, 0, 100, recvbuf, fsize, 'I');	

	int cc = fsize + 1;
	writeService->Write(recvbuf, cc);
	return 0;
}
*/
/*
int MMDevice::SendMUTE(BTHService *writeService)
{
	if (writeService == NULL) { return 1; }
	
	return 0;
}
*/
/*
int MMDevice::SendFrame(BTHService *writeService, char type)
{
	if (writeService == NULL) { return 1; }
	float fmin;
	float fmax;
	float fstep;
	float fval; // = pNotify->fMasterVolume;
	this->GetVolumeRange(&fmin, &fmax, &fstep);
	this->GetVolume(&fval);	
	char recvbuf[1024] = "";
	int fsize = 1024;
	this->CodeFrame(fval, fmin, fmax, recvbuf, fsize, type);	
	int cc = fsize + 1;
	return writeService->Write(recvbuf, cc);
	
}
*/
/*
int MMDevice::SendExitCode(BTHService *writeService)
{
	if (writeService == NULL) { return 1; }
	char recvbuf[8] = "BQE";
	writeService->Write(recvbuf, 8);
	return 0;
}
*/
