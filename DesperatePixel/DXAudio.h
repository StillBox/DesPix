#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include <Windows.h>
#include <XAudio2.h>
#include <assert.h>
#include <vector>
#include <map>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

//
//Sound classes
//

class DXAudio;

struct SoundState
{
	int	SoundID;
	int	SubmixID;
	int	SourceCount;
	std::vector<int> SourceBufferCount;
};

class Sound
{
	friend class DXAudio;

private:
	int									m_SoundID;
	int									m_SubmixID;
	BOOL								m_bSingle;
	WAVEFORMATEXTENSIBLE*				m_pFormat;
	std::vector<XAUDIO2_BUFFER*>		m_pBuffers;
	std::vector<IXAudio2SourceVoice*>	m_pSources;

public:
	Sound();
	~Sound();

	void	Register(int SoundID, int SubmixID = -1, BOOL bSingle = FALSE);
	void	Unregister();
	HRESULT AddSound(WCHAR* szFile, UINT nLoops = 0);
	HRESULT AddSoundFromRsrc(int ID, UINT nLoops = 0);
	HRESULT AddSource(IXAudio2SourceVoice** pSource);
	void	Play(int Index = -1, float fVolumn = 1.0f);
	void	SetVol(float fVolumn);
	void	Stop();
	void	End();

	void	GetState(SoundState* pState);
};

//
//Functions of Audio controlling
//

HRESULT InitDXAudio();
HRESULT ReleaseDXAudio();
IXAudio2* GetAudio();
HRESULT AddSubmix(UINT SubmixID);
IXAudio2SubmixVoice* GetSubmix(UINT SubmixID);

void SetMasterVolumn(float fVol);
float GetMasterVolumn();
void SetSubmixVolumn(UINT SubmixID, float fVol);
float GetSubmixVolumn(UINT SubmixID);

void DXAudioPlay(int SubmixID = -1);
void DXAudioStop(int SubmixID = -1);
void DXAudioEnd();

//
//Functions for reading WAV files
//

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);

#endif // !AUDIO_H
