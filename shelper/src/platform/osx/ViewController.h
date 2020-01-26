//
//  ViewController.h
//  test_osx
//
//  Created by Dmitry T on 18.01.2020.
//  Copyright © 2020 SocialQuantum. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <interop_mgr.h>

@interface ViewController : NSViewController
{
    shelper::interop_mgr_ptr m_interop_ptr;
    shelper::media_center::media_center_adapter_ptr m_mc;
}

@property (weak) IBOutlet NSTextFieldCell *m_host;
@property (weak) IBOutlet NSTextField *m_subtView;

-(void) onSubtitleText:(NSString*)text;


@end

