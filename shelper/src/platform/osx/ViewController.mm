//
//  ViewController.m
//  test_osx
//
//  Created by Dmitry T on 18.01.2020.
//  Copyright © 2020 SocialQuantum. All rights reserved.
//

#import "ViewController.h"
#import <kodi_adapter.h>
#import <output.h>

using namespace shelper;

class ios_output : public output::base_output {
    ViewController* m_ctrl;
public:
    ios_output(ViewController* ctrl) {
        m_ctrl = ctrl;
    }
    void set_text(const std::string& text) override {
        [m_ctrl onSubtitleText:[NSString stringWithUTF8String:text.c_str()]];
    }
};

@implementation ViewController

-(void)onTimer
{
    static auto t0 = std::chrono::steady_clock::now();
    auto t1 = std::chrono::steady_clock::now();
    std::chrono::duration<double> fs = t1 - t0;
    t0 = t1;
    m_interop_ptr->handle_timer(std::chrono::duration_cast<std::chrono::milliseconds>(fs).count());
    //self.m_subtView.stringValue = @"And they don't quit\
    //till they reel someone in.";
}

-(void) onSubtitleText:(NSString*)text {
    self.m_subtView.stringValue = text;
}
     
- (void)viewDidLoad {
    [super viewDidLoad];
    
    m_interop_ptr.reset(new interop_mgr());
    
    m_interop_ptr->set_output(std::make_shared<ios_output>(self));
    
    m_mc = std::make_shared<media_center::kodi_adapter>();
    m_interop_ptr->set_media_center(m_mc);
    
    m_mc->set_host([self.m_host.stringValue UTF8String]);
            
    [NSTimer scheduledTimerWithTimeInterval:0.05
      target:self
    selector:@selector(onTimer)
    userInfo:nil
     repeats:YES];
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void) openFiles
{
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    [panel setPrompt:@"Select subtitles"];
    NSArray* fileTypes = [NSArray arrayWithObjects:@"srt",nil];
    [panel setAllowedFileTypes:fileTypes];

    [panel setAllowsMultipleSelection:NO];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setFloatingPanel:NO];
    
    [panel beginWithCompletionHandler:^(NSInteger result){
        NSArray* files = [panel URLs];
        NSURL* u =  [files objectAtIndex:0];
        m_interop_ptr->load_subtitles([u.path UTF8String]);
        
    }];
}

- (IBAction)hostAction:(NSTextFieldCell *)sender {
    
    m_mc->set_host([sender.stringValue UTF8String]);
    
}
- (IBAction)onBtnLoadSbtFromFile:(NSButton *)sender {
    [self openFiles];
}

@end
