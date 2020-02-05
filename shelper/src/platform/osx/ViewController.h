//
//  ViewController.h
//  test_osx
//
//  Created by Dmitry T on 18.01.2020.
//  Copyright © 2020 SocialQuantum. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <interop_mgr.h>

@interface ViewController : NSViewController<NSTextFieldDelegate>
{
    shelper::interop_mgr_ptr m_interop_ptr;
    shelper::media_center::media_center_adapter_ptr m_mc;
    NSMutableArray<NSButton*>* m_words_buttons;
}

@property (weak) IBOutlet NSStackView *m_vertStackSub;

-(void)onSubtitleText:(NSString*)text;
-(void)onTranslateText:(NSString*)text;
-(void)onSrtFileSelected:(NSString*)file;
-(void)onHostChanged:(NSString*)host;
@end

