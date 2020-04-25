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
    NSMutableArray<UIButton*>* m_words_buttons;
}
@property (weak, nonatomic) IBOutlet UIStackView *m_vert_stack;

@property (weak, nonatomic) IBOutlet UIView *outputView;
@property (strong, nonatomic) IBOutlet UIView *baseView;

-(void)onSubtitleText:(NSString*)text;
-(void)onTranslateText:(NSString*)text;


@end

