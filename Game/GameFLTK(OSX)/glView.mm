//
//  glView.m
//  InfiniSurv
//
//  Created by Jody McAdams on 11/20/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#import "glView.h"
#include <OpenGL/gl.h>
#include "../InitGame.h"

@implementation glView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    
	if (self)
	{
        //blah
    }
	
    return self;
}


- (void)drawFrame
{
	
}



- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
	NSOpenGLContext    *currentContext = [self openGLContext];
	[currentContext makeCurrentContext];

	// must lock GL context because display link is threaded
	CGLLockContext((CGLContextObj)[currentContext CGLContextObj]);
	
	//const f32 timeStep = 1.0f/30.0f;
	
	const f32 actualTimeStep = CVDisplayLinkGetActualOutputVideoRefreshPeriod(m_displayLink);
	
	if(m_pGame)
	{
		NSPoint mouseLoc = [self convertPoint:[self.window convertScreenToBase:[NSEvent mouseLocation]]
					fromView:nil];
		
		NSRect rect = [self bounds];
		
		const BOOL outsideBounds = (mouseLoc.x < rect.origin.x || mouseLoc.x > rect.origin.x+rect.size.width
								   || mouseLoc.y < rect.origin.y || mouseLoc.y > rect.origin.y+rect.size.height);
		
		if(m_cursorIsHidden && outsideBounds)
		{
			m_cursorIsHidden = NO;
			CGDisplayShowCursor(CGMainDisplayID());
		}
		else if(!m_cursorIsHidden && !outsideBounds)
		{
			m_cursorIsHidden = YES;
			CGDisplayHideCursor(CGMainDisplayID());
		}

		if(m_pMouseInputState != NULL)
		{
			m_pMouseInputState->position.x = mouseLoc.x;
			m_pMouseInputState->position.y = mouseLoc.y;
			
			//TODO: make a less evil version of mouse sleep
			const bool mouseIsStuck = m_pMouseInputState->position.x == m_pMouseInputState->lastPosition.x 
			&& m_pMouseInputState->position.y == m_pMouseInputState->lastPosition.y;
			
			if(m_pMouseInputState->sleeping == false
			   && mouseIsStuck == true)
			{
				//COREDEBUG_PrintDebugMessage("Mouse fell asleep...");
				m_pMouseInputState->sleeping = true;
			}
			else if(m_pMouseInputState->sleeping == true
					&& mouseIsStuck == false)
			{
				//COREDEBUG_PrintDebugMessage("Mouse woke up!");
				m_pMouseInputState->sleeping = false;
			}
			
			NSUInteger mouseButtonState = [NSEvent pressedMouseButtons];
			if(mouseButtonState & 1 << 0)
			{
				if(m_pMouseInputState->buttonState[0] != CoreInput_ButtonState_Held)
				{
					m_pMouseInputState->buttonState[0] = CoreInput_ButtonState_Began;
				}
			}
			else
			{
				if(m_pMouseInputState->buttonState[0] != CoreInput_ButtonState_None)
				{
					m_pMouseInputState->buttonState[0] = CoreInput_ButtonState_Ended;
				}
			}
			
			if(mouseButtonState & 1 << 1)
			{
				if(m_pMouseInputState->buttonState[1] != CoreInput_ButtonState_Held)
				{
					m_pMouseInputState->buttonState[1] = CoreInput_ButtonState_Began;
				}
			}
			else
			{
				if(m_pMouseInputState->buttonState[1] != CoreInput_ButtonState_None)
				{
					m_pMouseInputState->buttonState[1] = CoreInput_ButtonState_Ended;
				}
			}
			
			/*if(m_pMouseInputState->buttonState[0] == CoreInput_ButtonState_Began)
			{
				m_pMouseInputState->buttonTimePressed[0] = 0.0f;
			}
			
			if(m_pMouseInputState->buttonState[1] == CoreInput_ButtonState_Began)
			{
				m_pMouseInputState->buttonTimePressed[1] = 0.0f;
			}
			
			if(m_pMouseInputState->buttonState[0] == CoreInput_ButtonState_Held)
			{
				m_pMouseInputState->buttonTimePressed[0] += actualTimeStep;
			}
			if(m_pMouseInputState->buttonState[1] == CoreInput_ButtonState_Held)
			{
				m_pMouseInputState->buttonTimePressed[1] += actualTimeStep;
			}*/
		}

		m_pGame->Update(actualTimeStep);
		m_pRenderer->Render(actualTimeStep);
	}
	
	glFlush();
	[currentContext flushBuffer];
	
	CGLUnlockContext((CGLContextObj)[currentContext CGLContextObj]);
    return kCVReturnSuccess;
}


- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (BOOL)becomeFirstResponder
{
	return YES;
}

- (BOOL)resignFirstResponder
{
	return YES;
}


-(void)keyUp:(NSEvent *)theEvent
{
	if(m_pKeyboardInputState == NULL)
	{
		return;
	}
	
	const u32 keyCode = theEvent.keyCode;
	
	const CoreInput_ButtonState currState = m_pKeyboardInputState->buttonState[keyCode];
	if(currState != CoreInput_ButtonState_None)
	{
		if(currState == CoreInput_ButtonState_Began)
		{
			m_pKeyboardInputState->skippedBegan[keyCode] = 1;
		}
		else
		{
			m_pKeyboardInputState->buttonState[keyCode] = CoreInput_ButtonState_Ended;
		}
	}
}


-(void)keyDown:(NSEvent *)theEvent
{
	if(m_pKeyboardInputState == NULL)
	{
		return;
	}
	
	const u32 keyCode = theEvent.keyCode;
	
	const CoreInput_ButtonState currState = m_pKeyboardInputState->buttonState[keyCode];
	if(currState != CoreInput_ButtonState_Held)
	{
		m_pKeyboardInputState->buttonState[keyCode] = CoreInput_ButtonState_Began;
	}
	
	switch(keyCode)
	{
		case 53: //Escape key
		{
			// Release the display link
			CVDisplayLinkRelease(m_displayLink);
			
			CleanUpGame(m_pGame,m_pRenderer);
			
			delete m_pRenderer;
			delete m_pGame;
			
			[NSApp terminate:self];
			
			break;
		}
	}
}


// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(__bridge glView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}




- (void)prepareOpenGL
{
	m_cursorIsHidden = NO;
	
	//[self.window setFrame:[[NSScreen mainScreen] frame] display:YES animate:NO];
	//[self.window setLevel:NSMainMenuWindowLevel+1];

	NSRect rect = [self bounds];
	rect.origin.x = 0;
	rect.origin.y = 0;
    rect.size = [self convertSize:rect.size toView:nil];
	glViewport(0.0, 0.0, NSWidth(rect), NSHeight(rect));
	
	s32 width;
	s32 height;
	
#if USE_IPHONE_RESOLUTION
	const f32 aspectRatio = rect.size.height/rect.size.width;
	height = 640;
	width = height/aspectRatio;
#else
	width = rect.size.width;
	height = rect.size.height;
#endif
	
	InitGame(&m_pGame, &m_pRenderer, width, height);
	m_pMouseInputState = &m_pGame->m_mouseState;
	m_pKeyboardInputState = &m_pGame->m_keyboardState;

    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; 
	
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&m_displayLink);
	
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(m_displayLink, &MyDisplayLinkCallback, (__bridge void*)self);
	
    // Set the display link for the current renderer
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(m_displayLink, cglContext, cglPixelFormat);
	
    // Activate the display link
    CVDisplayLinkStart(m_displayLink);
	
	[self.window setTitle:@"Crow-Regime"];
}


- (void)dealloc
{
    
	
	//[super dealloc];
}

@end
