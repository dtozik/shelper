//
//  ViewController.m
//  shelper
//
//  Created by Dmitry Tozik on 24.02.2020.
//  Copyright © 2020 Dmitry Tozik. All rights reserved.
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
        //[m_ctrl onSubtitleText:[NSString stringWithUTF8String:text.c_str()]];
    }
    
    void set_translation(const std::string& text) override {
        //[m_ctrl onTranslateText:[NSString stringWithUTF8String:text.c_str()]];
    }
};

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    m_interop_ptr.reset(new interop_mgr());
    m_interop_ptr->init();
    m_interop_ptr->set_output(std::make_shared<ios_output>(self));
    
//    m_mc = std::make_shared<media_center::kodi_adapter>();
//    m_interop_ptr->set_media_center(m_mc);
//    
//    m_mc->set_host("localhost:1234");
//#ifdef TEST_SUB
//    NSBundle* bundle = [NSBundle mainBundle];
//    NSURL* pathToExample = [bundle URLForResource:@TEST_SUB withExtension:@""];
//    if (pathToExample != nil)
//        m_interop_ptr->load_subtitles([pathToExample.path UTF8String]);
//#endif
//
//    [NSTimer scheduledTimerWithTimeInterval:0.05
//      target:self
//    selector:@selector(onTimer)
//    userInfo:nil
//     repeats:YES];
//    
//    m_words_buttons = [[NSMutableArray alloc] init];

}


@end
