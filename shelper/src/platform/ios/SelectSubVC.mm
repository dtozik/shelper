//
//  SelectSubVC.m
//  shelper
//
//  Created by Dmitry Tozik on 24.02.2020.
//  Copyright © 2020 Dmitry Tozik. All rights reserved.
//

#import "SelectSubVC.h"
#include <subtitles/fetcher.h>

using namespace shelper;

@interface SelectSubVC ()

@end

@implementation SelectSubVC



-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:
(NSIndexPath *)indexPath{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
    NSLog(@"Section:%d Row:%d selected and its data is %@",
          indexPath.section,indexPath.row,cell.textLabel.text);
	
	sub::request_callbacks clbs;
	clbs.complete = [self]() {
		
	};
	clbs.error = [](auto) {
		assert(false);
	};
	
	auto element = [m_delegate app]->fetcher()->get_subtitles_list()->at(indexPath.row);
	[m_delegate app]->fetcher()->download_subtitle_data(element.url, clbs);
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:
(NSInteger)section{
	auto list = [m_delegate app]->fetcher()->get_subtitles_list();
	if (!list)
		return 0;
	auto size = list->size();
	return size;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:
(NSIndexPath *)indexPath{
    static NSString* cellId = @"customcell";
    
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:
                             cellId];
    if (cell == nil) {
        cell = [[UITableViewCell alloc]initWithStyle:
                UITableViewCellStyleDefault reuseIdentifier:cellId];
    }
	
	auto list = [m_delegate app]->fetcher()->get_subtitles_list();
	auto element = list->at(indexPath.row);
	std::stringstream s;
	s << element.name << ' ' << element.score;
	[cell.textLabel setText:[NSString stringWithCString:s.str().c_str() encoding:NSUTF8StringEncoding]];
    return cell;
}


- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    if (textField == self.m_edit) {
        [textField resignFirstResponder];
		
		sub::request_callbacks clbs;
		clbs.complete = [self]() {
			[self.m_table reloadData];
		};
		clbs.error = [](auto) {
			assert(false);
		};
		
		[m_delegate app]->fetcher()->request_subtitles_list([textField.text UTF8String], clbs);
        return NO;
		//"https://dl.opensubtitles.org/en/download/src-api/vrf-19bc0c53/filead/1955363451.gz
    }
    return YES;
}

- (void)viewDidLoad {
    [super viewDidLoad];
	
	m_delegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
	
	self.m_edit.delegate = self;
	self.m_table.delegate = self;
	self.m_table.dataSource = self;
}
@end

