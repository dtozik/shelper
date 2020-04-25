//
//  SelectSubVC.h
//  shelper
//
//  Created by Dmitry Tozik on 24.02.2020.
//  Copyright © 2020 Dmitry Tozik. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <defs.h>

@interface SelectSubVC : UIViewController<UITableViewDataSource, UITableViewDelegate, UITextFieldDelegate>
{
	NSMutableArray *myArray;
	shelper::sub::subtitles_fetcher_ptr m_fetcher;
}
@property (weak, nonatomic) IBOutlet UITableView *m_table;
@property (weak, nonatomic) IBOutlet UITextField *m_edit;

@end


