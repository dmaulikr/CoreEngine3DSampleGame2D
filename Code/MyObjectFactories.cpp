#include "MyObjectFactories.h"
#include "Engine/CoreObjects/CoreObject_FactoryManager.h"
#include "Engine/Game/Game.h"

//ENGINE
#include "ENGINE/CoreObjects/Spawner.h"
#include "ENGINE/CoreObjects/Trigger_2D.h"
#include "ENGINE/CoreObjects/ScriptObject.h"
#include "ENGINE/CoreObjects/ObjectGroup.h"
#include "ENGINE/CoreObjects/TileAffector.h"
#include "ENGINE/CoreObjects/GUI/CoreUIImageView.h"
#include "ENGINE/CoreObjects/GUI/CoreUIButton.h"
#include "ENGINE/CoreObjects/SoundPlayer.h"
#include "ENGINE/CoreObjects/Effects/BasicParticle_2D.h"
#include "ENGINE/CoreObjects/Effects/MuzzleFlareParticle_2D.h"
#include "ENGINE/CoreObjects/Effects/AnimatedParticle_2D.h"
#include "ENGINE/CoreObjects/ScriptObject.h"
#include "ENGINE/CoreObjects/NullObject.h"
#include "ENGINE/CoreObjects/CurvePoints_2D.h"

//GAME
//TODO: put some game includes here

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void MyObjectFactories_Init()
{
	//Factory Managers that make life happy
	INITFACTORYMANAGER(GameObjects,2*SQUARE(1024));
	INITFACTORYMANAGER(GUI,SQUARE(1024)/2);
	
	//GAME
	
	
	//GUI (engine)
	ADDFACTORY(GUI,CoreUIView,8,false);
	ADDFACTORY(GUI,CoreUIImageView,32,false);
	ADDFACTORY(GUI,CoreUIButton,32,false);
	
	//ENGINE
	ADDFACTORY(GameObjects,Spawner,16,true);
	ADDFACTORY(GameObjects,Trigger_2D,64,true);
	ADDFACTORY(GameObjects,ScriptObject,32,true);
	ADDFACTORY(GameObjects,ObjectGroup,32,true);
	ADDFACTORY(GameObjects,TileAffector,8,true);
	ADDFACTORY(GameObjects,SoundPlayer,16,true);
	ADDFACTORY(GameObjects,BasicParticle_2D,2048,false);
	ADDFACTORY(GameObjects,AnimatedParticle_2D,32,false);
	ADDFACTORY(GameObjects,MuzzleFlareParticle_2D,16,false);
	ADDFACTORY(GameObjects,NullObject,32,true);
	ADDFACTORY(GameObjects,CurvePoints_2D,8,true);
	ADDFACTORY(GameObjects,Geometry_Normal,2048,false);
	ADDFACTORY(GameObjects,Geometry_UI,64,false);
	
	//Special care for these two
	g_Factory_Geometry_Normal.SetObjectsCanUpdate(false);
	g_Factory_Geometry_UI.SetObjectsCanUpdate(false);
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void MyObjectFactories_Clear()
{
	g_FactoryManager_GameObjects.Clear();
	g_FactoryManager_GUI.Clear();
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void MyObjectFactories_UpdateGUI(f32 timeElapsed)
{
	g_FactoryManager_GUI.Update(timeElapsed);
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void MyObjectFactories_Update(f32 timeElapsed)
{
	g_FactoryManager_GameObjects.Update(timeElapsed);
	
	//Resort geometry if something got deleted
	
	if(g_Factory_Geometry_Normal.m_objectHasBeenDeleted)
	{
		GLRENDERER->ForceRenderablesNeedSorting(RenderableObjectType_Normal);
	}
	
	if(g_Factory_Geometry_UI.m_objectHasBeenDeleted)
	{
		GLRENDERER->ForceRenderablesNeedSorting(RenderableObjectType_UI);
	}
}


