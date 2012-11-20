//
//  VisMasterViewController.h
//  visapp
//
//  Created by Oliver Carabia on 16/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class VisDetailViewController;

@interface VisMasterViewController : UITableViewController

@property (strong, nonatomic) VisDetailViewController *detailViewController;

@end
