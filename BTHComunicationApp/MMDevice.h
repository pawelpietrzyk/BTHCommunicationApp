#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#define MMD_ONNOTIFY		0x810
#define MMD_INIT_RECEIVED	0x801
#define MMD_EXIT_RECEIVED	0x802

#define MAX_VOL  100
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

//-----------------------------------------------------------
// Client implementation of IAudioEndpointVolumeCallback
// interface. When a method in the IAudioEndpointVolume
// interface changes the volume level or muting state of the
// endpoint device, the change initiates a call to the
// client's IAudioEndpointVolumeCallback::OnNotify method.
//-----------------------------------------------------------
class CAudioEndpointVolumeCallback : public IAudioEndpointVolumeCallback
{
    LONG _cRef;
public:
    CAudioEndpointVolumeCallback() :
        _cRef(1)
    {
    }
    ~CAudioEndpointVolumeCallback()
    {
    }
    // IUnknown methods -- AddRef, Release, and QueryInterface
    ULONG STDMETHODCALLTYPE AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        ULONG ulRef = InterlockedDecrement(&_cRef);
        if (0 == ulRef)
        {
            delete this;
        }
        return ulRef;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID **ppvInterface)
    {
        if (IID_IUnknown == riid)
        {
            AddRef();
            *ppvInterface = (IUnknown*)this;
        }
        else if (__uuidof(IAudioEndpointVolumeCallback) == riid)
        {
            AddRef();
            *ppvInterface = (IAudioEndpointVolumeCallback*)this;
        }
        else
        {
            *ppvInterface = NULL;
            return E_NOINTERFACE;
        }
        return S_OK;
    }

    // Callback method for endpoint-volume-change notifications.

    HRESULT STDMETHODCALLTYPE OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);
    
};
                
                
class MMDevice
{
 public:
    MMDevice(void);
    ~MMDevice(void);
    
    void GetVolumeRange(float *min, float *max, float *step);
    void GetVolume(float *volume);
	void SetVolumeUP(void);
	void SetVolumeDOWN(void);
    void SetVolume(float volume);
	void Mute(void);
	float DecodeFrame(char frame[], int frSize);
	void CodeFrame(float volume, float min, float max, char frame[], int &osize, char type);
	void CodeMsgVolume(float volume, char frame[], int &osize);
	void CodeMsgMUTE(bool mute, char frame[], int &osize);
	/*
	int SendFrame(BTHService *writeService, char type);
	void SendMsgVolume(BTHService *writeService);
	int SendInitRange(BTHService *writeService);
	int SendExitCode(BTHService *writeService);
	int SendMUTE(BTHService *writeService);
	*/
	//int SendExitCode(BTHConnection *bthCon);
	//void OnQuit(void);
 private:
    void Initialize(void);
    void Unitialize(void);
    IAudioEndpointVolume *masterAudioVolume;
    CAudioEndpointVolumeCallback EPVolEvents;
    
};

