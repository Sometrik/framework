//
//  FWLayoutView.h
//  Sometrik-project
//
//  Created by Sami Rämö on 17/04/2018.
//  Copyright © 2018 sometrik. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LayoutParams.h"

@interface FWLayoutView : UIView

@property (nonatomic, strong) NSMutableArray *items;

- (int)calcIntrinsicWidth:(UIView *)view;
- (int)calcIntrinsicHeight:(UIView *)view;
- (LayoutParams *)findParams:(UIView *)view;
- (void)relayoutAll;

- (void)addItem:(LayoutParams *)linearLayoutItem;
- (void)removeItem:(LayoutParams *)linearLayoutItem;
- (void)removeAllItems;

- (void)insertItem:(LayoutParams *)newItem beforeItem:(LayoutParams *)existingItem;
- (void)insertItem:(LayoutParams *)newItem afterItem:(LayoutParams *)existingItem;
- (void)insertItem:(LayoutParams *)newItem atIndex:(NSUInteger)index;

- (void)moveItem:(LayoutParams *)movingItem beforeItem:(LayoutParams *)existingItem;
- (void)moveItem:(LayoutParams *)movingItem afterItem:(LayoutParams *)existingItem;
- (void)moveItem:(LayoutParams *)movingItem toIndex:(NSUInteger)index;

- (void)swapItem:(LayoutParams *)firstItem withItem:(LayoutParams *)secondItem;

- (void)updateVisibility:(CGRect)bounds;

@end
