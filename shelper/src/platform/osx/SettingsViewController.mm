//
//  ViewController.m
//  test_osx
//
//  Created by Dmitry T on 18.01.2020.
//  Copyright © 2020 SocialQuantum. All rights reserved.
//

#import "SettingsViewController.h"
#import "ViewController.h"

@implementation SettingsViewController

- (void)viewDidLoad {
    [super viewDidLoad];
}

-(void)openFiles {
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    [panel setPrompt:@"Select subtitles"];
    NSArray* fileTypes = [NSArray arrayWithObjects:@"srt",nil];
    [panel setAllowedFileTypes:fileTypes];

    [panel setAllowsMultipleSelection:NO];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setFloatingPanel:NO];
    
    
    
    [panel beginSheetModalForWindow:[self.view window] completionHandler:^(NSModalResponse result) {
        NSArray* files = [panel URLs];
        NSURL* u =  [files objectAtIndex:0];
        [self.getVC onSrtFileSelected:u.path];
    }];
}

- (ViewController*)getVC {
    __block ViewController* vc = nil;

    [[NSApplication sharedApplication] enumerateWindowsWithOptions:NSWindowListOrderedFrontToBack usingBlock:^(NSWindow *window, BOOL *stop) {
        
        if ([[window contentViewController] isKindOfClass:ViewController.class]) {
            *stop = YES;
            vc = (ViewController*)[window contentViewController];
        }
    }];
    return vc;
}

- (IBAction)onLoadSrt:(NSButton *)sender {
    [self openFiles];
    
}

- (IBAction)onHostChange:(NSTextField *)sender {
    [self.getVC onHostChanged:sender.stringValue];
}

@end
