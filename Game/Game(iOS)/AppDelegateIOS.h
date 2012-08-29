//
//  AppDelegateIOS.h
//  InfiniSurv(iOS)
//
//  Created by Jody McAdams on 11/24/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) ViewController *viewController;

@end
