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
    
    void set_translation(const std::string& text) override {
        [m_ctrl onTranslateText:[NSString stringWithUTF8String:text.c_str()]];
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

     
- (void)viewDidLoad {
    [super viewDidLoad];
    
    m_interop_ptr.reset(new interop_mgr());
    m_interop_ptr->init();
    m_interop_ptr->set_output(std::make_shared<ios_output>(self));
    
    m_mc = std::make_shared<media_center::kodi_adapter>();
    m_interop_ptr->set_media_center(m_mc);
    
    m_mc->set_host("localhost:1234");
            
    [NSTimer scheduledTimerWithTimeInterval:0.05
      target:self
    selector:@selector(onTimer)
    userInfo:nil
     repeats:YES];
    
    m_words_buttons = [[NSMutableArray alloc] init];
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

-(void)onTranslateText:(NSString*)text {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:@"Translator"];
    [alert setInformativeText:text];
    [alert addButtonWithTitle:@"Ok"];
    [alert runModal];
}

-(void)onBtnWordTap:(NSButton*)btn {
    m_interop_ptr->on_pause();
}

-(void)onSubtitleText:(NSString*)text {

    text = [text stringByReplacingOccurrencesOfString:@"\r" withString:@"\r "];
    text = [text substringToIndex:[text length]-1];
    
    NSArray<NSString*>* array = [text componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    //array = [array filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:@"SELF != ''"]];
    
    [m_words_buttons removeAllObjects];
    [self.m_vertStackSub.subviews makeObjectsPerformSelector: @selector(removeFromSuperview)];
    
    NSStackView* curHorizStackView = [[NSStackView alloc] init];
    curHorizStackView.orientation = NSUserInterfaceLayoutOrientationHorizontal;
    curHorizStackView.distribution = NSStackViewDistributionFillProportionally;
    curHorizStackView.alignment = NSLayoutAttributeCenterY;
    [self.m_vertStackSub addView:curHorizStackView inGravity:NSStackViewGravityCenter];
    
    for (int i = 0; i < [array count]; ++i) {
        NSString* str = [array objectAtIndex:i];
        
        NSButton* button = nil;
        button = [[NSButton alloc] init];
        button.bezelStyle = NSBezelStyleRegularSquare;
        [button setButtonType:NSButtonTypePushOnPushOff];
        [button setAction:@selector(onBtnWordTap:)];
        [button setTarget:self];

        
        button.title = str;
        [curHorizStackView addView:button inGravity:NSStackViewGravityCenter];
        
        [m_words_buttons addObject:button];
        
        if ([str containsString:@"\r"]) {
            NSString* s = [str stringByReplacingOccurrencesOfString:@"\r" withString:@""];
            button.title = s;
           
            curHorizStackView = [[NSStackView alloc] init];
            curHorizStackView.orientation = NSUserInterfaceLayoutOrientationHorizontal;
            curHorizStackView.distribution = NSStackViewDistributionFillProportionally;
            curHorizStackView.alignment = NSLayoutAttributeCenterY;
            
            if (i < [array count]-1)
                [self.m_vertStackSub addView:curHorizStackView inGravity:NSStackViewGravityCenter];
        }

        
        
        
        //[self. addView:button inGravity:NSStackViewGravityCenter];
    }
    
    
}


-(void)onHostChanged:(NSString*)host {
    m_mc->set_host([host UTF8String]);
}

-(void)onSrtFileSelected:(NSString*)file {
    m_interop_ptr->load_subtitles([file UTF8String]);
}
- (IBAction)onBtnTranslate:(NSButton *)sender {
    std::string s;
    int counter = 0;
    for (NSButton* button in m_words_buttons) {
        if (button.state == NSControlStateValueOn) {
            if (counter == 1) {
                s += ' ';
            }
            s += [button.title UTF8String];
            counter++;
        }
    }
    
    m_interop_ptr->on_select_text(s);
}
@end
