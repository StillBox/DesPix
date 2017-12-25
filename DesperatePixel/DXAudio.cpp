#include "DXUT.h"
#include "DXAudio.h"

#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

//
//Audio Controls
//

class DXAudio
{
private:
	IXAudio2*								m_pXAudio;
	IXAudio2MasteringVoice*					m_pMasteringVoice;
	std::map<UINT, IXAudio2SubmixVoice*>	m_pSubmixVoices;
	std::map<UINT, Sound*>					m_pSounds;

public:
	DXAudio();
	~DXAudio();

	HRESULT		Init();
	HRESULT		Release();
	IXAudio2*	GetAudio() { return m_pXAudio; }

	void		AddSound(UINT SoundID, Sound *pSound);
	void		DelSound(UINT SoundID);
	Sound*		GetSound(UINT SoundID);

	HRESULT					AddSubmix(UINT SubmixID);
	IXAudio2SubmixVoice*	GetSubmix(UINT SubmixID);

	void		SetMasterVolumn(float fVol);
	float		GetMasterVolumn();
	void		SetSubmixVolumn(UINT SubmixID, float fVol);
	float		GetSubmixVolumn(UINT SubmixID);

	void		Play(int SubmixID = -1);
	void		Stop(int SubmixID = -1);
	void		End();
};

DXAudio::DXAudio()
{
	CoInitializeEx(NULL, 0);
	m_pXAudio = NULL;
	m_pMasteringVoice = NULL;
}

DXAudio::~DXAudio()
{
	Release();
	CoUninitialize();
}

HRESULT DXAudio::Init()
{
	HRESULT hr;
	HR(hr, XAudio2Create(&m_pXAudio));
	HR(hr, m_pXAudio->CreateMasteringVoice(&m_pMasteringVoice));
	return S_OK;
}

HRESULT DXAudio::Release()
{
	for (auto p : m_pSounds)
	{
		SAFE_DELETE(p.second);
	}

	auto iter = m_pSubmixVoices.begin();
	while (iter != m_pSubmixVoices.end())
	{
		iter->second->DestroyVoice();
		iter->second = NULL;
		iter++;
	}

	m_pMasteringVoice->DestroyVoice();
	SAFE_RELEASE(m_pXAudio);

	return S_OK;
}

void DXAudio::AddSound(UINT SoundID, Sound *pSound)
{
	auto iter = m_pSounds.find(SoundID);
	if (iter != m_pSounds.end())
		m_pSounds.erase(iter);
	m_pSounds.insert({ SoundID, pSound });
}

void DXAudio::DelSound(UINT SoundID)
{
	auto iter = m_pSounds.find(SoundID);
	if (iter != m_pSounds.end())
		m_pSounds.erase(iter);
}

Sound* DXAudio::GetSound(UINT SoundID)
{
	if (m_pSounds.find(SoundID) == m_pSounds.end())
	{
		Sound* pNewSound = new Sound();
		AddSound(SoundID, pNewSound);
	}
	return m_pSounds[SoundID];
}

HRESULT DXAudio::AddSubmix(UINT SubmixID)
{
	HRESULT hr;
	IXAudio2SubmixVoice *pNewSubmix = NULL;
	HR(hr, m_pXAudio->CreateSubmixVoice(&pNewSubmix, 2, 44100));
	m_pSubmixVoices.insert({ SubmixID, pNewSubmix });
	return TRUE;
}

IXAudio2SubmixVoice* DXAudio::GetSubmix(UINT SubmixID)
{
	if (m_pSubmixVoices.find(SubmixID) == m_pSubmixVoices.end())
		AddSubmix(SubmixID);
	return m_pSubmixVoices[SubmixID];
}

void DXAudio::SetMasterVolumn(float fVol)
{
	m_pMasteringVoice->SetVolume(fVol);
}

float DXAudio::GetMasterVolumn()
{
	float fVol;
	m_pMasteringVoice->GetVolume(&fVol);
	return fVol;
}

void DXAudio::SetSubmixVolumn(UINT SubmixID, float fVol)
{
	if (m_pSubmixVoices.find(SubmixID) == m_pSubmixVoices.end())
		AddSubmix(SubmixID);
	m_pSubmixVoices[SubmixID]->SetVolume(fVol);
}

float DXAudio::GetSubmixVolumn(UINT SubmixID)
{
	if (m_pSubmixVoices.find(SubmixID) == m_pSubmixVoices.end())
		AddSubmix(SubmixID);
	float fVol;
	m_pSubmixVoices[SubmixID]->GetVolume(&fVol);
	return fVol;
}

void DXAudio::Play(int SubmixID)
{
	for (auto p : m_pSounds)
		if (-1 == SubmixID || p.second->m_SubmixID == SubmixID)
			p.second->Play(-1);
}

void DXAudio::Stop(int SubmixID)
{
	for (auto p : m_pSounds)
		if (-1 == SubmixID || p.second->m_SubmixID == SubmixID)
			p.second->Stop();
}

void DXAudio::End()
{
	for (auto p : m_pSounds)
		p.second->End();
}

//Create a global pointer to DXAudio for manage the sounds

DXAudio *g_pDXAudio = new DXAudio();

DXAudio &GetDXAudio()
{
	assert(g_pDXAudio != NULL);
	return *g_pDXAudio;
}

HRESULT InitDXAudio()							{ return GetDXAudio().Init(); }
HRESULT ReleaseDXAudio()						{ return GetDXAudio().Release(); }
IXAudio2* GetAudio()							{ return GetDXAudio().GetAudio(); }
HRESULT AddSubmix(UINT SubmixID)				{ return GetDXAudio().AddSubmix(SubmixID); }
IXAudio2SubmixVoice* GetSubmix(UINT SubmixID)	{ return GetDXAudio().GetSubmix(SubmixID); }

void SetMasterVolumn(float fVol)				{ GetDXAudio().SetMasterVolumn(fVol); }
float GetMasterVolumn()							{ return GetDXAudio().GetMasterVolumn(); }
void SetSubmixVolumn(UINT SubmixID, float fVol) { GetDXAudio().SetSubmixVolumn(SubmixID, fVol); }
float GetSubmixVolumn(UINT SubmixID)			{ return GetDXAudio().GetSubmixVolumn(SubmixID); }

void DXAudioPlay(int SubmixID)					{ GetDXAudio().Play(SubmixID); }
void DXAudioStop(int SubmixID)					{ GetDXAudio().Stop(SubmixID); }
void DXAudioEnd()								{ GetDXAudio().End(); }

//
//Sound classes
//

Sound::Sound()
{
	m_SubmixID = -1;
	m_pFormat = NULL;
}

Sound::~Sound()
{
	Unregister();

	SAFE_DELETE(m_pFormat);

	for (auto p : m_pSources)
	{
		p->DestroyVoice();
		p = NULL;
	}
	std::vector<IXAudio2SourceVoice*>().swap(m_pSources);

	for (auto p : m_pBuffers)
	{
		SAFE_DELETE(p->pAudioData);
		SAFE_DELETE(p);
	}
	std::vector<XAUDIO2_BUFFER*>().swap(m_pBuffers);
}

HRESULT Sound::AddSound(WCHAR* szFile, UINT nLoops)
{
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkSize;
	DWORD dwChunkPosition;

	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

	m_pFormat = new WAVEFORMATEXTENSIBLE();
	XAUDIO2_BUFFER *pNewBuffer = new XAUDIO2_BUFFER();

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, m_pFormat, dwChunkSize, dwChunkPosition);

	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE * pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	CloseHandle(hFile);

	pNewBuffer->AudioBytes = dwChunkSize;
	pNewBuffer->pAudioData = pDataBuffer;
	pNewBuffer->Flags = XAUDIO2_END_OF_STREAM;
	pNewBuffer->LoopCount = nLoops;

	m_pBuffers.push_back(pNewBuffer);

	return S_OK;
}

HRESULT Sound::AddSoundFromRsrc(int ID, UINT nLoops)
{
	WCHAR szPath[MAX_PATH] = L"E:\\temp";
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	wcscat_s(szPath, L"\\Temp");
	if (!PathFileExists(szPath))
		CreateDirectory(szPath, NULL);
	wcscat_s(szPath, L"\\StillBox");
	if (!PathFileExists(szPath))
		CreateDirectory(szPath, NULL);
	wcscat_s(szPath, L"\\temp");

	HANDLE hFile = CreateFile(szPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HRSRC hrsc = FindResource(NULL, MAKEINTRESOURCE(ID), L"WAVE");
	DWORD dwErr;
	if (hrsc == NULL)
		dwErr = GetLastError();
	HGLOBAL hG = LoadResource(NULL, hrsc);
	DWORD dwSize = SizeofResource(NULL, hrsc);
	WriteFile(hFile, hG, dwSize, NULL, NULL);
	CloseHandle(hFile);

	AddSound(szPath, nLoops);
	//DeleteFile(szPath);

	return S_OK;
}

HRESULT Sound::AddSource(IXAudio2SourceVoice** ppSource)
{
	if (NULL == m_pFormat)
		return E_FAIL;

	if (NULL != (*ppSource))
	{
		(*ppSource)->DestroyVoice();
		(*ppSource) = NULL;
	}

	HRESULT hr;

	if (m_SubmixID != -1)
	{
		XAUDIO2_SEND_DESCRIPTOR SFXSend = { 0, GetSubmix(m_SubmixID) };
		XAUDIO2_VOICE_SENDS SFXSendList = { 1, &SFXSend };
		HR(hr, GetAudio()->CreateSourceVoice(ppSource, (WAVEFORMATEX*)m_pFormat,
			0, XAUDIO2_DEFAULT_FREQ_RATIO, 0, &SFXSendList, NULL));
	}
	else
	{
		HR(hr, GetAudio()->CreateSourceVoice(ppSource, (WAVEFORMATEX*)m_pFormat,
			0, XAUDIO2_DEFAULT_FREQ_RATIO, 0, 0, NULL));
	}

	if (m_bSingle)
	{
		for (auto p : m_pSources)
		{
			p->Stop(0);
			p->DestroyVoice();
		}
		std::vector<IXAudio2SourceVoice*>().swap(m_pSources);
	}
	else
	{
		auto iter = m_pSources.begin();
		while (iter != m_pSources.end())
		{
			XAUDIO2_VOICE_STATE State;
			(*iter)->GetState(&State);
			if (State.BuffersQueued == 0)
			{
				(*iter)->DestroyVoice();
				iter = m_pSources.erase(iter);
			}
			else
				iter++;
		}
	}
	m_pSources.push_back(*ppSource);

	return S_OK;
}

void Sound::Register(int SoundID, int SubmixID, BOOL bSingle)
{
	m_SoundID = SoundID;
	m_SubmixID = SubmixID;
	m_bSingle = bSingle;
	GetDXAudio().AddSound(SoundID, this);
}

void Sound::Unregister()
{
	GetDXAudio().DelSound(m_SoundID);
}

void Sound::Play(int Index, float fVolumn)
{
	if (Index >= 0)
	{
		IXAudio2SourceVoice *pNewSource = NULL;
		AddSource(&pNewSource);
		pNewSource->SubmitSourceBuffer(m_pBuffers[Index]);
		pNewSource->SetVolume(fVolumn);
		pNewSource->Start(0);
	}
	else
	{
		for (auto p : m_pSources)
			p->Start(0);
	}
}

void Sound::SetVol(float fVolumn)
{
	for (auto p : m_pSources)
		p->SetVolume(fVolumn);
}

void Sound::Stop()
{
	for (auto p : m_pSources)
		p->Stop(0);
}

void Sound::End()
{
	for (auto p : m_pSources)
	{
		p->Stop(0);
		p->DestroyVoice();
	}
	std::vector<IXAudio2SourceVoice*>().swap(m_pSources);
}

void Sound::GetState(SoundState* pState)
{
	pState->SoundID = m_SoundID;
	pState->SubmixID = m_SubmixID;
	pState->SourceCount = m_pSources.size();
	for (int i = 0; i < pState->SourceCount; i++)
	{
		if (pState->SourceBufferCount.size() <= i)
			pState->SourceBufferCount.push_back(0);

		XAUDIO2_VOICE_STATE sourceState;
		m_pSources[i]->GetState(&sourceState);
		pState->SourceBufferCount[i] = sourceState.BuffersQueued;
	}
}

//
//Functions for reading WAV files
//

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}