//
//  AppDelegate.m
//  shelper
//
//  Created by Dmitry Tozik on 24.02.2020.
//  Copyright © 2020 Dmitry Tozik. All rights reserved.
//

#import "AppDelegate.h"
#include <downloader.h>
#include <fs/posix_file_system.h>

using namespace shelper;

@interface AppDelegate ()
@end

@implementation AppDelegate


-(std::shared_ptr<app>)app {
	return self->m_app;
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
	
	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
	NSString* cacheDirectory = [paths objectAtIndex:0];
	
	NSBundle* bundle = [NSBundle mainBundle];
	NSString* data = [bundle resourcePath];
	
	paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory,
														 NSUserDomainMask,
														 YES);
	NSString* profile_dir = [[paths objectAtIndex:0] stringByAppendingPathComponent:[bundle bundleIdentifier]];
	
	[[NSFileManager defaultManager] createDirectoryAtPath:profile_dir withIntermediateDirectories:YES attributes:nil error:nil];
	
	auto fs = std::make_shared<fs::posix_file_system>([cacheDirectory UTF8String],
													  [data UTF8String],
													  [profile_dir UTF8String]);
	
	m_app = std::make_shared<app>(downloader, fs);
	m_app->init();
	
	[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
	
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
