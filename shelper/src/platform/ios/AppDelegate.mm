//
//  AppDelegate.m
//  shelper
//
//  Created by Dmitry Tozik on 24.02.2020.
//  Copyright © 2020 Dmitry Tozik. All rights reserved.
//

#import "AppDelegate.h"
#include <downloader.h>

using namespace shelper;

@interface AppDelegate ()
@end

@implementation AppDelegate


-(std::shared_ptr<app>)app {
	return self.app;
}


-(void)onTimer {
    static auto t0 = std::chrono::steady_clock::now();
    auto t1 = std::chrono::steady_clock::now();
    std::chrono::duration<double> fs = t1 - t0;
    t0 = t1;
    m_app->handle_timer(std::chrono::duration_cast<std::chrono::milliseconds>(fs).count());
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
		
	auto downloader = std::make_shared<net::downloader>([](auto clb) {
		dispatch_async(dispatch_get_main_queue(), ^{
			clb();
		});
	});
	
	m_app = std::make_shared<app>(downloader);
	m_app->init();
	
    return YES;
}


#pragma mark - UISceneSession lifecycle


- (UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options {
    // Called when a new scene session is being created.
    // Use this method to select a configuration to create the new scene with.
    return [[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role];
}


- (void)application:(UIApplication *)application didDiscardSceneSessions:(NSSet<UISceneSession *> *)sceneSessions {
    // Called when the user discards a scene session.
    // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
    // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
}


@end
