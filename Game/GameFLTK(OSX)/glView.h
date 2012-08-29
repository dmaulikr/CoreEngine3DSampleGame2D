//
//  glView.h
//  InfiniSurv
//
//  Created by Jody McAdams on 11/20/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#import <AppKit/AppKit.h>
#import <QuartzCore/QuartzCore.h>

#include "Engine/OpenGLRenderer.h"
#include "Game.h"

#include "CoreInput_PCInputState.h"

#define USE_IPHONE_RESOLUTION 1

@interface glView : NSOpenGLView
{
	CVDisplayLinkRef m_displayLink;
	OpenGLRenderer* m_pRenderer;
	Game* m_pGame;
	
	float    viewWidth;
	float    viewHeight;
	
	BOOL m_cursorIsHidden;
	
	MouseInputState* m_pMouseInputState;
	KeyboardInputState* m_pKeyboardInputState;
}

- (void)drawFrame;

- (BOOL)acceptsFirstResponder;
- (BOOL)becomeFirstResponder;
- (BOOL)resignFirstResponder;

@end
