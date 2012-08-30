//
//  InitGame.cpp
//  InfiniSurv(OSX)
//
//  Created by Jody McAdams on 11/29/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "InitGame.h"
#include "../Code/CustomGame.h"

void InitGame(Game** ppGame, OpenGLRenderer** ppRenderer, s32 width, s32 height)
{
	*ppRenderer = new OpenGLRenderer;
	
	int widthPoints = width;
	int heightPoints = height;
	
#if defined(PLATFORM_IOS)
	if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
	{
		//Convert iPhone 3GS points to iPhone 4 points
		widthPoints = 960;
		heightPoints = 640;
	}
	else
	{
		widthPoints = 1024;
		heightPoints = 768;
	}
#endif
	
	(*ppRenderer)->Init(width, height, widthPoints, heightPoints);
	
	//Create game
	*ppGame = (Game*)new SampleGame2D;
	
	//Init game
	(*ppGame)->Init();

	//Create all materials that the game will use for rendering
	(*ppRenderer)->CreateMaterials();
}

void CleanUpGame(Game* pGame, OpenGLRenderer* pRenderer)
{
	pGame->CleanUp();
	pRenderer->CleanUp();
}
