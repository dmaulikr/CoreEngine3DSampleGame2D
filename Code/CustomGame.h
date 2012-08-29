//
//  InfiniSurv.h
//  InfiniSurv(OSX)
//
//  Created by Jody McAdams on 11/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef InfiniSurv_OSX__InfiniSurv_h
#define InfiniSurv_OSX__InfiniSurv_h

#include "Engine/Graphics/OpenGLRenderer.h"
#include "Engine/Game/Game.h"
#include "Engine/CoreObjects/CoreObject.h"
#include "Engine/Animation/AnimationController.h"
#include "Engine/Input/CoreInput_GamePad.h"

#include <string>


#define GAME_DISPLAYED_TILESIZE_DIV 2

#define GAME_MAX_PLAYERS 1

#define GAME_MAX_CAM_DIST_TABLE_ENTRIES 128
#define GAME_MAX_CAM_POINTS 64

class SampleGame2D: public Game
{
public:
	enum GameState
	{
		GameState_Invalid,
		GameState_FadingIn,
		GameState_Playing,
		GameState_WatchingDeath,
		GameState_FadingOut,
	};
	
	enum TitleScreenState
	{
		TitleScreenState_WaitingToFadeOut,
		TitleScreenState_FadingOut,
		TitleScreenState_Idle,
	};
	
	enum CamState
	{
		CamState_Normal,
	};
	
	virtual bool Init();
	virtual bool LoadSharedResources();
	virtual void Update(f32 timeElapsed);
	virtual void CleanUp();
	virtual void LoadResourcesForType(u32 type);
	virtual void ProcessMessage(u32 message, u32 parameter);
	
	virtual const vec3* GetPlayerPos(s32 playerIndex);
	
	void ChangeState(GameState newState);
    void UpdateMainGame(f32 timeElapsed);
	
protected:
	virtual void HandlePauseStatus();
	
private:
	void LoadLevel();

	f32 m_view[16];
	
	vec3 m_camPos;

	GamePad m_pad[GAME_MAX_PLAYERS];
	
	GameState m_state;
	CamState m_camState;
	
	CoreObjectHandle m_hTitleScreen;
	TitleScreenState m_titleScreenState;
	f32 m_timeScreenTimer;
	
	u32 m_pauseSoundSource;
	
	vec3 m_lastCamPos;
};


#endif
