//
//  ViewController.h
//  shelper
//
//  Created by Dmitry Tozik on 24.02.2020.
//  Copyright © 2020 Dmitry Tozik. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <interop_mgr.h>
#import "AppDelegate.h"

@interface ViewController : UIViewController
{
    NSMutableArray<UIButton*>* m_words_buttons;
	AppDelegate* m_delegate;
}
@property (weak, nonatomic) IBOutlet UIStackView *m_vert_stack;

@property (weak, nonatomic) IBOutlet UIView *outputView;
@property (strong, nonatomic) IBOutlet UIView *baseView;

-(void)onSubtitleText:(NSString*)text;
-(void)onTranslateText:(NSString*)text;


@end

