//
//  ViewController.h
//  shelper
//
//  Created by Dmitry Tozik on 24.02.2020.
//  Copyright © 2020 Dmitry Tozik. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <interop_mgr.h>

@interface ViewController : UIViewController
{
    shelper::interop_mgr_ptr m_interop_ptr;
    shelper::media_center::media_center_adapter_ptr m_mc;
    //NSMutableArray<NSButton*>* m_words_buttons;
}
@end

