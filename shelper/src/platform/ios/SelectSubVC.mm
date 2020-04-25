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
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:
(NSInteger)section{
	auto list = m_fetcher->get_subtitles_list();
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
	
	auto list = m_fetcher->get_subtitles_list();
	auto element = list->at(indexPath.row);
	std::stringstream s;
	s << element.name << ' ' << element.score;
	[cell.textLabel setText:[NSString stringWithCString:s.str().c_str() encoding:NSUTF8StringEncoding]];
    return cell;
}

- (void)complete {
	[self.m_table reloadData];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    if (textField == self.m_edit) {
        [textField resignFirstResponder];
		
		request_callbacks clbs;
		clbs.complete = [self]() {
			[self performSelectorOnMainThread:@selector(complete) withObject:nil waitUntilDone:NO];
		};
		clbs.error = [](auto) {
			
		};
		m_fetcher->request_subtitles_list([textField.text UTF8String], clbs);
		
		
        return NO;
    }
    return YES;
}

- (void)viewDidLoad {
    [super viewDidLoad];
	
	self.m_edit.delegate = self;
	m_fetcher = std::make_shared<sub::fetcher>();
	
	self.m_table.delegate = self;
	self.m_table.dataSource = self;
}
@end

