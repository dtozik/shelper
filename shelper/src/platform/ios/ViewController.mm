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
#include <downloader.h>
#include <translator.h>

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
    
    void set_translation(const std::string& text, const std::string& translation, const std::string& context) override {
        [m_ctrl onTranslateText:[NSString stringWithUTF8String:text.c_str()]
					translation:[NSString stringWithUTF8String:translation.c_str()]
					context:[NSString stringWithUTF8String:context.c_str()]];
    }
};

@interface ViewController ()

@end

@implementation ViewController


-(void)onBtnWordTap:(UIButton*)btn {
	[btn setSelected:![btn isSelected]];
	[m_delegate app]->interop()->on_pause();
}

- (void)viewDidDisappear:(BOOL)animated {
	[m_delegate app]->interop()->set_output(nullptr);
}

- (void)viewDidLoad {
    [super viewDidLoad];
	
    m_delegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
	[m_delegate app]->interop()->set_output(std::make_shared<ios_output>(self));
	
    m_words_buttons = [[NSMutableArray alloc] init];

	UISwipeGestureRecognizer* rightSwipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeRight:)];
	[rightSwipe setDirection: UISwipeGestureRecognizerDirectionRight ];
	[self.baseView addGestureRecognizer:rightSwipe];

	UITapGestureRecognizer *singleFingerTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tap:)];
	[self.baseView addGestureRecognizer:singleFingerTap];
}

-(void)swipeRight:(UISwipeGestureRecognizer*)gesture {
	[m_delegate app]->interop()->on_backward();
}


- (void)tap:(UITapGestureRecognizer *)recognizer {
    media_center::media_center_adapter_ptr adapter = [m_delegate app]->interop()->get_media_center();
    if (adapter->is_playing()) {
        [m_delegate app]->interop()->on_pause();
    } else {
        [m_delegate app]->interop()->on_play();
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
    media_center::media_center_adapter_ptr adapter = [m_delegate app]->interop()->get_media_center();
    if (adapter->is_playing()) {
        [m_delegate app]->interop()->on_pause();
    } else {
        [m_delegate app]->interop()->on_play();
    }
}

- (IBAction)onStopBtn:(id)sender {
	[m_delegate app]->interop()->on_stop();
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
        [m_delegate app]->interop()->on_select_text(s);
}

-(void)onTranslateText:(NSString*)text translation:(NSString*)translation context:(NSString*)context {
	UIAlertController* alert = [UIAlertController alertControllerWithTitle:text
							   message:translation
							   preferredStyle:UIAlertControllerStyleAlert];

	UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
								   handler:^(UIAlertAction * action) {
		for (UIButton* button in m_words_buttons) {
			[button setSelected:NO];
		}
	}];
	
	std::string txt = [text UTF8String];
	std::string trans = [translation UTF8String];
	std::string ctx = [context UTF8String];
	UIAlertAction* addToDict = [UIAlertAction actionWithTitle:@"Add" style:UIAlertActionStyleDefault
								   handler:^(UIAlertAction * action) {
		for (UIButton* button in m_words_buttons) {
			[button setSelected:NO];
		}
		[m_delegate app]->interop()->get_translator()->store(txt, trans, ctx);
	}];

	[alert addAction:defaultAction];
	[alert addAction:addToDict];
	[self presentViewController:alert animated:YES completion:nil];
}

@end
