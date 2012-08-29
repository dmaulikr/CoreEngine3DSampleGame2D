//
//  CustomGame.cpp
//  CoreEngine3D
//
//  Created by Jody McAdams on 11/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "CustomGame.h"

#include "Engine/Graphics/MaterialDeclarations.h"
#include "Engine/Math/MathUtil.h"
#include "Engine/Math/matrix.h"
#include "Engine/Util/CoreDebug.h"

#include "MyObjectFactories.h"
#include "GameMessages.h"
#include "SharedResources.h"

#include "Hash.h"

#include <iostream>

#if defined (PLATFORM_OSX) || defined (PLATFORM_IOS)
#import <Foundation/Foundation.h>
#endif

#include "MyTypes.h"

const bool g_MUSIC_Enabled = true;
const bool g_SOUND_Enabled = true;

//Compare functions
bool Compare_SpawnableEntities_X(const SpawnableEntity& lhs, const SpawnableEntity& rhs);

s32 g_SongID_Normal = 0;
s32 g_SongID_Boss = 0;

static const u32 g_Player_CamY_NumMoveCurvePoints = 5;
static const f32 g_Player_CamY_MoveCurvePoints[g_Player_CamY_NumMoveCurvePoints] =
{
	0.0f,
	0.2f,
	0.5f,
	0.9f,
	1.0f,
};

static const f32 g_Game_CameraSpeed_Normal = 128.0f;

bool SampleGame2D::Init()
{
    std::cout << "main: done" << std::endl; 
    
	Game::Init();
	
	m_pauseSoundSource = 0;
	
	Box2D_SetDefaultCollisionFriction(0.4f);
	Box2D_SetNumIterations(1,1);
	
	m_camState = CamState_Normal;
	
	m_state = GameState_Invalid;
	
	m_titleScreenState = TitleScreenState_WaitingToFadeOut;
	m_timeScreenTimer = 0.0f;
	
	GAME->Box2D_Init(false, true);
	 
	SetTileCullingRange(3);
	
	OPENALAUDIO->SetMaxSoundDistance(200.0f);
	
	SetParallaxScale(0.3f);
	
	MyObjectFactories_Init();
	
	//g_SongID_SpaceCat = AddSongToPlaylist("ArtResources/Music/03 Phat With a PhD.mp3");
	g_SongID_Normal = AddSongToPlaylist("ArtResources/Music/gundamwing.mp3");
	g_SongID_Boss = AddSongToPlaylist("ArtResources/Music/EggBoss.mp3");
#if defined (PLATFORM_IOS) || defined (PLATFORM_ANDROID)
	m_pad[0].Init(&m_deviceInputState);
#else
	m_pad[0].Init(NULL);
#endif
    
#if defined (PLATFORM_IOS)
	const f32 controlScaleStick = [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone ? 1.0f:0.45f;
	const f32 controlScaleButtons = [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone ? 1.0f:0.55f;
#else
	const f32 controlScaleStick = 1.0f;
	const f32 controlScaleButtons = 1.0f;
#endif
    
	const f32 buttonWidth = 160.0f*controlScaleButtons;
	const f32 buttonRadius = 48.0f*controlScaleButtons;
    
	const JoystickBoundingBox stickBoundingBox = {0.0f,GLRENDERER->screenWidth_points-buttonWidth*2.0f,0.0f,(f32)GLRENDERER->screenHeight_points};


	
	m_pad[0].AddJoystick(GamePadJoystickType_AnalogLeft,JoystickMode_TouchRelative, stickBoundingBox, 64.0f*controlScaleStick, 8.0f*controlScaleStick, 0.01f, 0.0f);
	
	
	const JoystickBoundingBox buttonBB_A = {(f32)GLRENDERER->screenWidth_points-buttonWidth,(f32)GLRENDERER->screenWidth_points,0.0f,(f32)GLRENDERER->screenHeight_points};
	m_pad[0].AddButton(GamePadButtonType_Face_Left,buttonBB_A, buttonRadius);
	
	const JoystickBoundingBox buttonBB_B = {(f32)GLRENDERER->screenWidth_points-buttonWidth*2.0f,(f32)GLRENDERER->screenWidth_points-buttonWidth,0.0f,(f32)GLRENDERER->screenHeight_points};
	m_pad[0].AddButton(GamePadButtonType_Face_Down,buttonBB_B, buttonRadius);
	

	//GLRENDERER->SetClearColor(0.494f, 0.6784f, 0.94f);
	GLRENDERER->SetClearColor(0.0f, 0.0f, 0.0f);
	
	//GLRENDERER->LoadParticleAtlas("ArtResources/Textures/ParticleAtlas_01.png");
	
	mat4f_LoadIdentity(m_view);
    GLRENDERER->SetViewMatrixForIndex(m_view, 0);

	OPENALAUDIO->SetListenerOrientation(&vec3_away, &vec3_up);

	if(g_SOUND_Enabled == true)
	{
		OPENALAUDIO->SetListenerVolume(1.0f);
	}
	else
	{
		OPENALAUDIO->SetListenerVolume(0.0f);
	}
	
	//Set up UI Context
	
	OPENALAUDIO->SwitchContext(GameAudioContext_UI);
	
	OPENALAUDIO->SetMaxSoundDistance(200.0f);
	
	OPENALAUDIO->SetListenerOrientation(&vec3_away, &vec3_up);
	OPENALAUDIO->SetListenerVelocity(&vec3_zero);
	
	if(g_SOUND_Enabled == true)
	{
		OPENALAUDIO->SetListenerVolume(1.0f);
	}
	else
	{
		OPENALAUDIO->SetListenerVolume(0.0f);
	}
	
	OPENALAUDIO->SwitchContext(GameAudioContext_Normal);
	
	LoadLevel();
    
    ChangeState(GameState_FadingIn);

	return true;
}

void SampleGame2D::LoadLevel()
{
	Reset();
	
	/*std::string levelPath("ArtResources/TestLevel/");
	
	//std::string levelFile("TestMap.tmx");
	std::string levelFile("TestMapTurningCamera.tmx");
	LoadTiledLevelFromTMX(levelPath,levelFile,64,1.0f);
	Array_InsertionSort(m_spawnableEntities,m_numSpawnableEntities,Compare_SpawnableEntities_X);
	*/
	
	GAME->Box2D_TogglePhysicsDebug(true);
	
	/*std::string uiPath("ArtResources/UI/");
	std::string titleScreenFile("TitleScreenView.xml");
	
	CoreUIView* pTitleView = LoadCoreUIFromXML(uiPath, titleScreenFile);
	m_hTitleScreen = pTitleView->GetHandle();*/

	CopyVec3(&m_lastCamPos,&vec3_zero);
	CopyVec3(&m_camPos, &vec3_zero);
	
	m_camState = CamState_Normal;
	
	if(g_MUSIC_Enabled)
	{
		//PlaySongByID(g_SongID_Normal, 0.5f, true);
	}
	
	m_titleScreenState = TitleScreenState_WaitingToFadeOut;
	m_timeScreenTimer = 0.0f;
	
	if(m_pauseSoundSource == 0)
	{
		/*OPENALAUDIO->SwitchContext(GameAudioContext_UI);
		OPENALAUDIO->CreateSoundBufferFromFile(g_SoundAsset_Pause.soundFileName,&g_SoundAsset_Pause.soundBufferID);
		m_pauseSoundSource = OPENALAUDIO->CreateSoundSourceFromBuffer(g_SoundAsset_Pause.soundBufferID);
		OPENALAUDIO->SwitchContext(GameAudioContext_Normal);*/
	}
}


const vec3* SampleGame2D::GetPlayerPos(s32 playerIndex)
{
	return NULL;
	//return m_player->GetPosition();
}


bool SampleGame2D::LoadSharedResources()
{
	//Textures
	
	//Sounds
	
	return true;
}


void SampleGame2D::ProcessMessage(u32 message, u32 parameter)
{
	switch(message)
	{
		case GameMessage_Player1Died:
		{
			if(m_state != GameState_FadingOut)
			{
				ChangeState(GameState_FadingOut);
			}
			
			break;
		}
	}
}


void SampleGame2D::ChangeState(GameState newState)
{
	//UNINIT
	switch(m_state)
	{
		case GameState_FadingIn:
		{
			break;
		}
		case GameState_Playing:
		{
			break;
		}
		case GameState_WatchingDeath:
		{
			break;
		}
		case GameState_FadingOut:
		{
            MyObjectFactories_Clear();
            
            COREOBJECTMANAGER->Clear();
            
            Box2D_ResetWorld();
            
            ClearItemArt();
            ClearItemSounds();
            
            LoadLevel();
            
			break;
		}
		default:
		{
			break;
		}
	}
	
	//INIT
	switch(newState)
	{
		case GameState_FadingIn:
		{
			GLRENDERER->ForceFadeIn(2.0f);
			
			break;
		}
		case GameState_Playing:
		{
			break;
		}
		case GameState_WatchingDeath:
		{
			
			
			break;
		}
		case GameState_FadingOut:
		{
			GAME->StopSong();
            
			GLRENDERER->StartFadeOut(1.0f);
			
			break;
		}
		default:
		{
			break;
		}
	}
	
	m_state = newState;
}

void SampleGame2D::HandlePauseStatus()
{
	if(m_paused)
	{
		SetGlobalSongVolume(0.0f);
		
		OPENALAUDIO->SwitchContext(GameAudioContext_UI);
		OPENALAUDIO->PlaySoundSource(m_pauseSoundSource, 1.0f, 1.0f, false);
	}
	else
	{
		OPENALAUDIO->SwitchContext(GameAudioContext_Normal);
		SetGlobalSongVolume(1.0f);
	}
}

void SampleGame2D::UpdateMainGame(f32 timeElapsed)
{
	//Always update GUI first
	MyObjectFactories_UpdateGUI(timeElapsed);
	
    if(m_paused == true)
	{
		//If it's paused, just do a quick game update and bust out
		Game::Update(timeElapsed);
		
		return;
	}
	
	const f32 maxTimeElapsed = 1.0f/30.0f;
	if(timeElapsed > maxTimeElapsed)
	{
		timeElapsed = maxTimeElapsed;
	}
    
	switch(m_camState)
	{
		case CamState_Normal:
		{
			ConstrainCameraToTiledLevel();

			break;
		}
	}
	
	mat4f_SetLookAtPos(m_view,&m_camPos);
	
	//update and debug draw joystick
	for(u32 i=0; i<GAME_MAX_PLAYERS; ++i)
	{
#if defined (PLATFORM_IOS) || defined (PLATFORM_ANDROID)
		m_pad[i].Update();
		m_pad[i].DebugDraw();
#endif
		
#if defined (PLATFORM_OSX) || defined (PLATFORM_WIN)
		//TODO: some XBOX controller stuff here
#endif
		
	}
	
	//TODO: update player
	
	//Update all the object factories (which in turn updates each object)
	MyObjectFactories_Update(timeElapsed);
	
	CoreUIView* pTitleView = (CoreUIView*)COREOBJECTMANAGER->GetObjectByHandle(m_hTitleScreen);
	if(pTitleView != NULL)
	{
		switch(m_titleScreenState)
		{
			case TitleScreenState_WaitingToFadeOut:
			{
				m_timeScreenTimer += timeElapsed*2.0f;
				if(m_timeScreenTimer > 2.0f)
				{
					m_titleScreenState = TitleScreenState_FadingOut;
				}
				break;
			}
			case TitleScreenState_FadingOut:
			{
				pTitleView->fadeAlpha -= timeElapsed;
				if(pTitleView->fadeAlpha <= 0.0f)
				{
					pTitleView->fadeAlpha = 0.0f;

					m_titleScreenState = TitleScreenState_Idle;
				}
				
				pTitleView->LayoutView();
				
				break;
			}
			case TitleScreenState_Idle:
			{
				break;
			}
		}
	}

	//Only update the direction if the normalize doesn't fail
	vec3 tempCamDir;
	SubVec3(&tempCamDir, &m_camPos, &m_lastCamPos);
	
	//TODO: care what direction the camera is going
	
	m_lastCamPos = m_camPos;
}


void SampleGame2D::Update(f32 timeElapsed)
{
#if defined(_DEBUG_PC)
	//Toggle GUI editor (in-progress)
	if(m_keyboardState.buttonState[103] == CoreInput_ButtonState_Ended)
	{
		ToggleUIEditor();
	}
#endif
	
	//UPDATE!
	switch(m_state)
	{
		case GameState_FadingIn:
		{
            UpdateMainGame(timeElapsed);
            
            if(GLRENDERER->GetFadeFinished())
			{
				ChangeState(GameState_Playing);
			}
            
			break;
		}
		case GameState_Playing:
		{
            UpdateMainGame(timeElapsed);
            
			break;
		}
		case GameState_WatchingDeath:
		{
			break;
		}
		case GameState_FadingOut:
		{
            UpdateMainGame(timeElapsed);
            
			if(GLRENDERER->GetFadeFinished())
			{
				ChangeState(GameState_FadingIn);
			}
			
			break;
		}
		default:
		{
			break;
		}
	}
	
	//Do this here unless you want to die
	Game::Update(timeElapsed);
}


void SampleGame2D::LoadResourcesForType(u32 type)
{
    switch (type)
	{
        default:
        {
            break;
        }
    }
}

void SampleGame2D::CleanUp()
{
	Game::CleanUp();
}

bool Compare_SpawnableEntities_X(const SpawnableEntity& lhs, const SpawnableEntity& rhs)
{
	//TODO: needs to incorporate enemy scale
	return lhs.position.x-lhs.scale.x <= rhs.position.x-rhs.scale.x;
}
