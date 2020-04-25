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
        [m_ctrl onSubtitleText:[NSString stringWithUTF8String:text.c_str()]];
    }
    
    void set_translation(const std::string& text) override {
        [m_ctrl onTranslateText:[NSString stringWithUTF8String:text.c_str()]];
    }
};

@interface ViewController ()

@end

@implementation ViewController


-(void)onTimer
{
    static auto t0 = std::chrono::steady_clock::now();
    auto t1 = std::chrono::steady_clock::now();
    std::chrono::duration<double> fs = t1 - t0;
    t0 = t1;
    m_interop_ptr->handle_timer(std::chrono::duration_cast<std::chrono::milliseconds>(fs).count());
}

-(void)onBtnWordTap:(UIButton*)btn {
	[btn setSelected:![btn isSelected]];
    m_interop_ptr->on_pause();
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    m_interop_ptr.reset(new interop_mgr());
    m_interop_ptr->init();
    m_interop_ptr->set_output(std::make_shared<ios_output>(self));
    
    m_mc = std::make_shared<media_center::kodi_adapter>();
    m_interop_ptr->set_media_center(m_mc);
    
    m_mc->set_host("192.168.1.181:1234");
#ifdef TEST_SUB
    NSBundle* bundle = [NSBundle mainBundle];
    NSURL* pathToExample = [bundle URLForResource:@TEST_SUB withExtension:@""];
    if (pathToExample != nil)
        m_interop_ptr->load_subtitles([pathToExample.path UTF8String]);
#endif

    [NSTimer scheduledTimerWithTimeInterval:0.05
      target:self
    selector:@selector(onTimer)
    userInfo:nil
     repeats:YES];
    
    m_words_buttons = [[NSMutableArray alloc] init];

	UISwipeGestureRecognizer* rightSwipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeRight:)];
	[rightSwipe setDirection: UISwipeGestureRecognizerDirectionRight ];
	[self.baseView addGestureRecognizer:rightSwipe];

	UITapGestureRecognizer *singleFingerTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tap:)];
	[self.baseView addGestureRecognizer:singleFingerTap];
}

-(void)swipeRight:(UISwipeGestureRecognizer*)gesture {
	m_interop_ptr->on_backward();
}


- (void)tap:(UITapGestureRecognizer *)recognizer {
    media_center::media_center_adapter_ptr adapter = m_interop_ptr->get_media_center();
    if (adapter->is_playing()) {
        m_interop_ptr->on_pause();
    } else {
        m_interop_ptr->on_play();
    }
}

-(void)onSubtitleText:(NSString*)text {

    text = [text stringByReplacingOccurrencesOfString:@"\r" withString:@"\r "];
    text = [text substringToIndex:[text length]-1];
    
    NSArray<NSString*>* array = [text componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    
    [m_words_buttons removeAllObjects];
	[self.outputView.subviews makeObjectsPerformSelector: @selector(removeFromSuperview)];
    //[self.m_vert_stack.subviews makeObjectsPerformSelector: @selector(removeFromSuperview)];
    
    //UIStackView* curHorizStackView = [[UIStackView alloc] init];
    //curHorizStackView.axis = UILayoutConstraintAxisHorizontal;
    //curHorizStackView.distribution = UIStackViewDistributionFillProportionally;
    //curHorizStackView.alignment = UIStackViewAlignmentCenter;
    //[self.m_vert_stack addSubview:curHorizStackView];
	int offset_x = 0;
	int offset_y = 0;
	int base_height = -1;
	CGRect output_rc = self.outputView.frame;
    for (int i = 0; i < [array count]; ++i) {
        NSString* str = [array objectAtIndex:i];
        
        UIButton* button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		button.backgroundColor = [UIColor colorWithCGColor:CGColorCreateSRGB(0.9, 0.9, 0.9, 1)];
		[button addTarget:self action:@selector(onBtnWordTap:)forControlEvents:UIControlEventTouchDown];
		//button.frame = CGRectMake(0.0, 0.0, 160.0, 40.0);
		
		bool shift_y = false;
        if ([str containsString:@"\r"]) {
            str = [str stringByReplacingOccurrencesOfString:@"\r" withString:@""];
			if (i < [array count]-1)
				shift_y = true;
        }
		
		[button setTitle:str forState:UIControlStateNormal];
		[button sizeToFit];
		CGRect rc = button.frame;
		
		if (base_height == -1)
			base_height = rc.size.height;
		else
			rc.size.height = base_height;
	

		
		if (offset_x + rc.size.width + 5 > output_rc.size.width || shift_y) {
			offset_x = 0;
			offset_y += base_height + 5;
		}
		
		rc.origin.x = offset_x;
		rc.origin.y = offset_y;
		button.frame = rc;
		offset_x += rc.size.width + 5;
		
		
		[self.outputView addSubview:button];
        [m_words_buttons addObject:button];
    }
}
- (IBAction)onPlayPauseBtn:(id)sender {
    media_center::media_center_adapter_ptr adapter = m_interop_ptr->get_media_center();
    if (adapter->is_playing()) {
        m_interop_ptr->on_pause();
    } else {
        m_interop_ptr->on_play();
    }
}

- (IBAction)onStopBtn:(id)sender {
	m_interop_ptr->on_stop();
}

- (IBAction)onBtnTranslate:(id)sender {
    std::string s;
    int counter = 0;
    for (UIButton* button in m_words_buttons) {
        if ([button isSelected]) {
            if (counter == 1) {
                s += ' ';
            }
            s += [button.currentTitle UTF8String];
            counter++;
        }
    }
    
    if (s.size())
        m_interop_ptr->on_select_text(s);
}

-(void)onTranslateText:(NSString*)text {
	UIAlertController* alert = [UIAlertController alertControllerWithTitle:@""
							   message:text
							   preferredStyle:UIAlertControllerStyleAlert];

	UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
								   handler:^(UIAlertAction * action) {
		for (UIButton* button in m_words_buttons) {
			[button setSelected:NO];
		}
	}];

	[alert addAction:defaultAction];
	[self presentViewController:alert animated:YES completion:nil];
}

@end
