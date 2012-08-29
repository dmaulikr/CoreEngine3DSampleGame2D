//
//  InitGame.h
//  InfiniSurv(OSX)
//
//  Created by Jody McAdams on 11/29/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef InfiniSurv_OSX__InitGame_h
#define InfiniSurv_OSX__InitGame_h

#include "Engine/Graphics/OpenGLRenderer.h"
#include "Engine/Game/Game.h"

void InitGame(Game** ppGame, OpenGLRenderer** ppRenderer, s32 width, s32 height);
void CleanUpGame(Game* pGame, OpenGLRenderer* pRenderer);
#endif
