//
//  FWLayoutView.m
//  Sometrik-project
//
//  Created by Sami Rämö on 17/04/2018.
//  Copyright © 2018. All rights reserved.
//

#import "FWLayoutView.h"
#import "FrameLayoutView.h"
#import "LinearLayoutView.h"
#import "FWScrollView.h"

@interface FWLayoutView()
- (void)setup;
@end

@implementation FWLayoutView

- (id)init {
    self = [super init];
    if (self) {
        [self setup];
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self setup];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self setup];
    }
    return self;
}

- (void)setup {
    self.items = [[NSMutableArray alloc] init];
    self.autoresizesSubviews = NO;
    self.translatesAutoresizingMaskIntoConstraints = false;
}

- (int)calcIntrinsicWidth:(UIView *)view {
    if ([view isKindOfClass:LinearLayoutView.class]) {
        LinearLayoutView * child = (LinearLayoutView*)view;
        int width = 0;
        if (child.orientation == LinearLayoutViewOrientationHorizontal) {
            for (LayoutParams *item in child.items) {
                if (item.view.hidden) continue;
                
                width += item.margin.left + item.margin.right;
                if (item.fixedWidth > 0) {
                    width += item.fixedWidth;
                } else {
                    width += [self calcIntrinsicWidth:item.view] + item.padding.left + item.padding.right;
                }
            }
        } else {
            for (LayoutParams *item in child.items) {
                if (item.view.hidden) continue;
                
                int w = 0;
                if (item.fixedWidth > 0) {
                    w = item.fixedWidth + item.margin.left + item.margin.right;
                } else {
                    w = [self calcIntrinsicWidth:item.view] + item.padding.left + item.padding.right + item.margin.left + item.margin.right;
                }
                if (w > width) width = w;
            }
        }
        return width;
    } else if ([view isKindOfClass:FrameLayoutView.class]) {
        FrameLayoutView * child = (FrameLayoutView*)view;
        int width = 0;
        for (LayoutParams *item in child.items) {
            if (item.view.hidden) continue;
            
            int w = 0;
            if (item.fixedWidth > 0) {
                w = item.fixedWidth + item.margin.left + item.margin.right;
            } else {
                w = [self calcIntrinsicWidth:item.view] + item.padding.left + item.padding.right + item.margin.left + item.margin.right;
            }
            if (w > width) width = w;
        }
        return width;
    } else {
        return view.intrinsicContentSize.width;
    }
}

- (int)calcIntrinsicHeight:(UIView *)view {
    if ([view isKindOfClass:LinearLayoutView.class]) {
        LinearLayoutView * child = (LinearLayoutView*)view;
        int height = 0;
        if (child.orientation == LinearLayoutViewOrientationVertical) {
            for (LayoutParams *item in child.items) {
                if (item.view.hidden) continue;
                
                height += item.margin.top + item.margin.bottom;
                if (item.fixedHeight > 0) {
                    height += item.fixedHeight;
                } else {
                    height += [self calcIntrinsicHeight:item.view] + item.padding.top + item.padding.bottom;
                }
            }
        } else {
            for (LayoutParams *item in child.items) {
                if (item.view.hidden) continue;
                
                int h = 0;
                if (item.fixedHeight > 0) {
                    h = item.fixedHeight + item.margin.top + item.margin.bottom;
                } else {
                    h = [self calcIntrinsicHeight:item.view] + item.padding.top + item.padding.bottom + item.margin.top + item.margin.bottom;
                }
                if (h > height) height = h;
            }
        }
        return height;
    } else if ([view isKindOfClass:FrameLayoutView.class]) {
        FrameLayoutView * child = (FrameLayoutView*)view;
        int height = 0;
        for (LayoutParams *item in child.items) {
            if (item.view.hidden) continue;
            
            int h = 0;
            if (item.fixedHeight > 0) {
                h = item.fixedHeight + item.margin.top + item.margin.bottom;
            } else {
                h = [self calcIntrinsicHeight:item.view] + item.padding.top + item.padding.bottom + item.margin.top + item.margin.bottom;
            }
            if (h > height) height = h;
        }
        return height;
    } else {
        return view.intrinsicContentSize.height;
    }
}

- (LayoutParams*)findParams:(UIView *)view {
    for (LayoutParams *item in self.items) {
        if (item.view == view) {
            return item;
        }
    }
    return nil;
}

- (void)relayoutAll {
    UIView * view = self;
    while (view != nil && ([view isKindOfClass:FWLayoutView.class] || [view isKindOfClass:FWScrollView.class])) {
        [view setNeedsLayout];
#if 0
        if ([view isKindOfClass:FWScrollView.class]) {
            break;
        }
#endif
        view = view.superview;
    }
}

- (void)addItem:(LayoutParams *)item {
    if (item == nil || [self.items containsObject:item] == YES || item.view == nil) {
        return;
    }
    
    [self.items addObject:item];
    [self addSubview:item.view];

    item.topConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    item.leftConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    item.rightConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    item.bottomConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    item.centerXConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeCenterX relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeCenterX multiplier:1.f constant:0.0f];
    item.centerYConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeCenterY relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeCenterY multiplier:1.f constant:0.0f];
    item.widthConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:0];
    item.heightConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:0];
    item.maxWidthConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationLessThanOrEqual toItem:self attribute:NSLayoutAttributeWidth multiplier:1.0f constant:0];
    item.maxHeightConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationLessThanOrEqual toItem:self attribute:NSLayoutAttributeHeight multiplier:1.0f constant:0];

    item.topConstraint.priority = 999 - item.level;
    item.leftConstraint.priority = 999 - item.level;
    item.rightConstraint.priority = 999 - item.level;
    item.bottomConstraint.priority = 999 - item.level;
    item.centerXConstraint.priority = 999 - item.level;
    item.centerYConstraint.priority = 999 - item.level;
    item.widthConstraint.priority = 999 - item.level - 1;
    item.heightConstraint.priority = 999 - item.level - 1;
    item.maxWidthConstraint.priority = 999 - item.level;
    item.maxHeightConstraint.priority = 999 - item.level;

    [self relayoutAll];
}

- (void)removeItem:(LayoutParams *)item {
    if (item == nil || [self.items containsObject:item] == NO) {
        return;
    }
    
    [item.view removeFromSuperview];
    [self.items removeObject:item];
    [self relayoutAll];
}

- (void)removeAllItems {
    // only remove actual items, not scrollbars
    for (LayoutParams *item in self.items) {
        [item.view removeFromSuperview];
    }
    [self.items removeAllObjects];
    [self relayoutAll];
}

- (void)insertItem:(LayoutParams *)newItem beforeItem:(LayoutParams *)existingItem {
    if (newItem == nil || [self.items containsObject:newItem] == YES || existingItem == nil ||  [self.items containsObject:existingItem] == NO) {
        return;
    }
    
    NSUInteger index = [self.items indexOfObject:existingItem];
    [self.items insertObject:newItem atIndex:index];
    [self addSubview:newItem.view];
    [self relayoutAll];
}

- (void)insertItem:(LayoutParams *)newItem afterItem:(LayoutParams *)existingItem {
    if (newItem == nil || [self.items containsObject:newItem] == YES || existingItem == nil || [self.items containsObject:existingItem] == NO) {
        return;
    }
    
    if (existingItem == [self.items lastObject]) {
        [self.items addObject:newItem];
    } else {
        NSUInteger index = [self.items indexOfObject:existingItem];
        [self.items insertObject:newItem atIndex:++index];
    }
    
    [self addSubview:newItem.view];
    [self relayoutAll];
}

- (void)insertItem:(LayoutParams *)newItem atIndex:(NSUInteger)index {
    if (newItem == nil || [self.items containsObject:newItem] == YES || index >= [self.items count]) {
        return;
    }
    
    [self.items insertObject:newItem atIndex:index];
    [self addSubview:newItem.view];
    [self relayoutAll];
}

- (void)moveItem:(LayoutParams *)movingItem beforeItem:(LayoutParams *)existingItem {
    if (movingItem == nil || [self.items containsObject:movingItem] == NO || existingItem == nil || [self.items containsObject:existingItem] == NO || movingItem == existingItem) {
        return;
    }
    
    [self.items removeObject:movingItem];
    
    NSUInteger existingItemIndex = [self.items indexOfObject:existingItem];
    [self.items insertObject:movingItem atIndex:existingItemIndex];
    
    [self setNeedsLayout];
}

- (void)moveItem:(LayoutParams *)movingItem afterItem:(LayoutParams *)existingItem {
    if (movingItem == nil || [self.items containsObject:movingItem] == NO || existingItem == nil || [self.items containsObject:existingItem] == NO || movingItem == existingItem) {
        return;
    }
    
    [self.items removeObject:movingItem];
    
    if (existingItem == [self.items lastObject]) {
        [self.items addObject:movingItem];
    } else {
        NSUInteger existingItemIndex = [self.items indexOfObject:existingItem];
        [self.items insertObject:movingItem atIndex:++existingItemIndex];
    }
    
    [self setNeedsLayout];
}

- (void)moveItem:(LayoutParams *)movingItem toIndex:(NSUInteger)index {
    if (movingItem == nil || [self.items containsObject:movingItem] == NO || index >= [self.items count] || [self.items indexOfObject:movingItem] == index) {
        return;
    }
    
    [self.items removeObject:movingItem];
    
    if (index == ([self.items count] - 1)) {
        [self.items addObject:movingItem];
    } else {
        [self.items insertObject:movingItem atIndex:index];
    }
    
    [self setNeedsLayout];
}

- (void)swapItem:(LayoutParams *)firstItem withItem:(LayoutParams *)secondItem {
    if (firstItem == nil || [self.items containsObject:firstItem] == NO || secondItem == nil || [self.items containsObject:secondItem] == NO || firstItem == secondItem) {
        return;
    }
    
    NSUInteger firstItemIndex = [self.items indexOfObject:firstItem];
    NSUInteger secondItemIndex = [self.items indexOfObject:secondItem];
    [self.items exchangeObjectAtIndex:firstItemIndex withObjectAtIndex:secondItemIndex];
    
    [self setNeedsLayout];
}

@end
