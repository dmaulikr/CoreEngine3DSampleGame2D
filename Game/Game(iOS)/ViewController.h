//
//  ViewController.h
//  Game(iOS)
//
//  Created by Jody McAdams on 7/28/12.
//
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include "Engine/Game/Game.h"
#include "Engine/Graphics/OpenGLRenderer.h"
#include "Engine/Input/TouchInputIOS.h"

@interface ViewController : GLKViewController
{
	Game* m_pGame;
	OpenGLRenderer* m_pRenderer;
	TouchInputIOS* m_pTouchInput;
}
@end
