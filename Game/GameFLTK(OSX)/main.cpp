//
//  main.cpp
//  SampleGame2D FLTK(OSX)
//
//  Created by Jody McAdams on 11/24/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include "Engine/Game/FLTKGLWindow.h"

#include "../InitGame.h"

OpenGLRenderer* m_pRenderer;
Game* m_pGame;

MouseInputState* m_pMouseInputState;
KeyboardInputState* m_pKeyboardInputState;

int g_window_width = 960;
int g_window_height = 640;

int main(int argc, char *argv[])
{
	FLTKGLWindow win( 0,0,g_window_width,g_window_height,"SampleGame2D FLTK (OSX)",60.0f,true );
    win.begin();
	Fl_Button but( 10, 150, 70, 30, "Click me" );
    win.end();
    //but.callback( but_cb );
    win.show();
	
	win.SetCursorHiddenWhenOnScreen(true);
	
	InitGame(&m_pGame, &m_pRenderer, g_window_width, g_window_height);
	m_pMouseInputState = &m_pGame->m_mouseState;
	m_pKeyboardInputState = &m_pGame->m_keyboardState;
	
    return Fl::run();
}
