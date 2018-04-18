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


@end
