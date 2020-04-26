//
//  AppDelegate.h
//  shelper
//
//  Created by Dmitry Tozik on 24.02.2020.
//  Copyright © 2020 Dmitry Tozik. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <app.h>

using namespace shelper;
@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
	std::shared_ptr<app> m_app;
}
-(std::shared_ptr<app>)app;

@end

