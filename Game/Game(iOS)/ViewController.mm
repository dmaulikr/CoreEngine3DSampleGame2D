//
//  ViewController.m
//  InfiniSurv(iOS)
//
//  Created by Jody McAdams on 11/24/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#import "ViewController.h"

//TODO: fix this crappy include
#if defined(PLATFORM_WIN)
#include "../../Game/InitGame.h"
#else
#include "../InitGame.h"
#endif

@interface ViewController () {
    GLuint _program;
    
    GLKMatrix4 _modelViewProjectionMatrix;
    GLKMatrix3 _normalMatrix;
    float _rotation;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL:(GLKView*)glkView;
- (void)tearDownGL;

- (BOOL)loadShaders;
- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file;
- (BOOL)linkProgram:(GLuint)prog;
- (BOOL)validateProgram:(GLuint)prog;
@end

@implementation ViewController

@synthesize context = _context;
@synthesize effect = _effect;

- (void)dealloc
{

}


//TOUCH CODE
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[m_pTouchInput touchesBegan:touches withEvent:event :self.view];
}


-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	[m_pTouchInput touchesMoved:touches withEvent:event :self.view];
}


-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	[m_pTouchInput touchesEnded:touches withEvent:event :self.view];
}


-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	[m_pTouchInput touchesCancelled:touches withEvent:event :self.view];
}
//END TOUCH CODE


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
		//Failed to create context
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableMultisample = GLKViewDrawableMultisampleNone;
    view.drawableStencilFormat = GLKViewDrawableStencilFormatNone;
    view.drawableDepthFormat = GLKViewDrawableDepthFormatNone;
    
	self.preferredFramesPerSecond = 60;
    
	
    [self setupGL:view];

	//Link to the touch input from the game
	m_pTouchInput = m_pGame->m_pTouchInput;
}


- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
	
	CleanUpGame(m_pGame,m_pRenderer);
	
	delete m_pRenderer;
	delete m_pGame;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}


- (void)setupGL:(GLKView*)glkView
{
    [EAGLContext setCurrentContext:self.context];

	const f32 screenWidthPoints = self.view.bounds.size.width;
	const f32 screenHeightPoints = self.view.bounds.size.height;
	
	InitGame(&m_pGame, &m_pRenderer, screenWidthPoints, screenHeightPoints);
    
    //Bind the screen frame buffer
    [glkView bindDrawable];
    
    //Get the frame buffer
    u32 frameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (int*)&frameBuffer);
    
    //Tell the renderer about it
    m_pRenderer->SetScreenFramebuffer(frameBuffer);
}


- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}


#pragma mark - GLKView and GLKViewController delegate methods


- (void)update
{
	m_pGame->Update(self.timeSinceLastUpdate);

	[m_pTouchInput PostGameUpdate];

}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	if(m_pRenderer != NULL)
	{
		m_pRenderer->Render(self.timeSinceLastUpdate);
	}
}

#pragma mark -  OpenGL ES 2 shader compilation


- (BOOL)loadShaders
{
	return YES;
}


- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
{	return YES;
}


- (BOOL)linkProgram:(GLuint)prog
{
	return YES;
}


- (BOOL)validateProgram:(GLuint)prog
{
    return YES;
}


@end
