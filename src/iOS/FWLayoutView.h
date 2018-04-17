//
//  FWLayoutView.h
//  Sometrik-project
//
//  Created by Sami Rämö on 17/04/2018.
//  Copyright © 2018 manjamedia. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LayoutParams.h"

@interface FWLayoutView : UIView

@property (nonatomic, strong) NSMutableArray *items;

- (int)calcIntrinsicWidth:(UIView *)view;
- (int)calcIntrinsicHeight:(UIView *)view;
- (LayoutParams *)findParams:(UIView *)view;
- (void)relayoutAll;

@end
