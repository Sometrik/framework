#import "LinearLayoutView.h"

#import "FrameLayoutView.h"

@interface LinearLayoutView()
- (void)setup;
@end

@implementation LinearLayoutView

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
    _items = [[NSMutableArray alloc] init];
    _orientation = LinearLayoutViewOrientationVertical;
    self.autoresizesSubviews = NO;
    self.translatesAutoresizingMaskIntoConstraints = false;
}

- (void)layoutSubviews {
    LayoutParams * myParams = nil;
    if ([self.superview isKindOfClass:LinearLayoutView.class]) {
        LinearLayoutView * layout = (LinearLayoutView*)self.superview;
        myParams = [layout findParams:self];
    } else if ([self.superview isKindOfClass:FrameLayoutView.class]) {
        FrameLayoutView * layout = (FrameLayoutView*)self.superview;
        myParams = [layout findParams:self];
    }

    CGFloat paddingLeft = 0, paddingRight = 0, paddingTop = 0, paddingBottom = 0;
    if (myParams != nil) {
        paddingLeft = myParams.padding.left;
        paddingRight = myParams.padding.right;
        paddingTop = myParams.padding.top;
        paddingBottom = myParams.padding.bottom;
    }

    CGFloat relativePosition = 0.0;
    CGFloat extraSpace = 0.0;

    if (self.orientation == LinearLayoutViewOrientationVertical) {
        extraSpace = self.frame.size.height - paddingTop - paddingBottom;
        relativePosition = paddingTop;
    } else {
        extraSpace = self.frame.size.width - paddingLeft - paddingRight;
        relativePosition = paddingLeft;
    }
    
    for (LayoutParams *item in _items) {
        if (item.view.hidden) {
            continue;
        }
        int size;
        if (self.orientation == LinearLayoutViewOrientationVertical) {
            if (item.fixedHeight > 0) {
                size = item.fixedHeight + item.margin.top + item.margin.bottom;
            } else {
                size = [self calcIntrinsicHeight:item.view] + item.margin.top + item.margin.bottom + item.padding.top + item.padding.bottom;
            }
        } else {
            if (item.fixedWidth > 0) {
                size = item.fixedWidth + item.margin.left + item.margin.right;
            } else {
                size = [self calcIntrinsicWidth:item.view] + item.margin.left + item.margin.right + item.padding.left + item.padding.right;
            }
        }
        extraSpace -= size;
    }

    if (extraSpace < 0) {
        extraSpace = 0;
    } else {
        extraSpace /= [_items count];
    }

    extraSpace = 0;
    
    for (LayoutParams *item in _items) {
        if (item.view.hidden) {
            continue;
        }

        CGFloat startMargin = 0.0;
        CGFloat endMargin = 0.0;
        CGFloat currentDimension = 0.0;

        CGFloat minWidth, minHeight;

        if (item.fixedWidth > 0) {
            minWidth = item.fixedWidth;
        } else {
            minWidth = [self calcIntrinsicWidth:item.view] + item.padding.left + item.padding.right;
        }
        if (item.fixedHeight > 0) {
            minHeight = item.fixedHeight;
        } else {
            minHeight = [self calcIntrinsicHeight:item.view] + item.padding.top + item.padding.bottom;
        }

        if (self.orientation == LinearLayoutViewOrientationHorizontal) {
            minWidth += extraSpace;

            startMargin = item.margin.left;
            endMargin = item.margin.right;
                    
            relativePosition += startMargin;

            item.leftConstraint.constant = relativePosition;
            item.widthConstraint.constant = minWidth;
            item.leftConstraint.active = YES;
            item.widthConstraint.active = YES;
            item.rightConstraint.active = NO;
            item.centerXConstraint.active = NO;
	    
            if (item.fixedHeight == -1) {
                item.topConstraint.constant = paddingTop + item.margin.top;
                item.bottomConstraint.constant = -(paddingBottom + item.margin.bottom);
                item.topConstraint.active = YES;
                item.bottomConstraint.active = YES;
                item.centerYConstraint.active = NO;
                item.heightConstraint.active = NO;
                item.maxHeightConstraint.active = NO;
            } else {
                item.maxHeightConstraint.constant = -(paddingTop + item.margin.top + paddingBottom + item.margin.bottom);
                item.maxHeightConstraint.active = YES;

                item.heightConstraint.constant = minHeight;
                item.heightConstraint.active = YES;

                if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentTop) {
                    item.topConstraint.constant = paddingTop + item.margin.top;
                    item.topConstraint.active = YES;
                    item.bottomConstraint.active = NO;
                    item.centerYConstraint.active = NO;
                } else if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentBottom) {
                    item.bottomConstraint.constant = -(paddingBottom + item.margin.bottom);
                    item.topConstraint.active = NO;
                    item.bottomConstraint.active = YES;
                    item.centerYConstraint.active = NO;
                } else { // LinearLayoutItemVerticalCenter
                    // absolutePosition = (self.frame.size.height / 2) - ((height + (item.margin.bottom - item.margin.top)) / 2);
                    item.topConstraint.active = NO;
                    item.bottomConstraint.active = NO;
                    item.centerYConstraint.active = YES;
                }
	        }

            currentDimension = minWidth;
        } else {           
            minHeight += extraSpace;

            startMargin = item.margin.top;
            endMargin = item.margin.bottom;
            
            relativePosition += startMargin;

            item.topConstraint.constant = relativePosition;
            item.heightConstraint.constant = minHeight;
            item.topConstraint.active = YES;
            item.bottomConstraint.active = NO;
            item.centerYConstraint.active = NO;
            item.heightConstraint.active = YES;
            
            if (item.fixedWidth == -1) {
                item.leftConstraint.constant = paddingLeft + item.margin.left;
                item.rightConstraint.constant = -(paddingRight + item.margin.right);
                item.leftConstraint.active = YES;
                item.rightConstraint.active = YES;
                item.centerXConstraint.active = NO;
                item.widthConstraint.active = NO;
                item.maxWidthConstraint.active = NO;
            } else {
                item.maxWidthConstraint.constant = -(paddingLeft + item.margin.left + paddingRight + item.margin.right);
                item.maxWidthConstraint.active = YES;

                item.widthConstraint.constant = minWidth;
                item.widthConstraint.active = YES;

                if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentLeft) {
                    item.leftConstraint.constant = paddingLeft + item.margin.left;
                    item.leftConstraint.active = YES;
                    item.rightConstraint.active = NO;
                    item.centerXConstraint.active = NO;
                } else if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentRight) {
                    item.rightConstraint.constant = -(paddingRight + item.margin.right);
                    item.leftConstraint.active = NO;
                    item.rightConstraint.active = YES;
                    item.centerXConstraint.active = NO;
                } else { // LinearLayoutItemHorizontalCenter
                    // absolutePosition = (self.frame.size.width / 2) - ((width + (item.margin.right - item.margin.left)) / 2);
                    item.leftConstraint.active = NO;
                    item.rightConstraint.active = NO;
                    item.centerXConstraint.active = YES;
                }
	        }
            currentDimension = minHeight;
        }

        relativePosition += currentDimension + endMargin;
    }

    for (CALayer * layer in self.layer.sublayers) {
      layer.frame = self.bounds;
    }
}

- (void)addSubview:(UIView *)view {
    [super addSubview:view];
    [self setNeedsLayout];
    [self.superview setNeedsLayout];
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

- (void)addItem:(LayoutParams *)item {
    if (item == nil || [_items containsObject:item] == YES || item.view == nil) {
        return;
    }
    
    [_items addObject:item];
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
}

- (void)removeItem:(LayoutParams *)item {
    if (item == nil || [_items containsObject:item] == NO) {
        return;
    }
    
    [item.view removeFromSuperview];
    [_items removeObject:item];
}

- (void)removeAllItems {
    // only remove actual items, not scrollbars
    for (LayoutParams *item in self.items) {
        [item.view removeFromSuperview];
    }
    [self.items removeAllObjects];
}

- (void)insertItem:(LayoutParams *)newItem beforeItem:(LayoutParams *)existingItem {
    if (newItem == nil || [_items containsObject:newItem] == YES || existingItem == nil ||  [_items containsObject:existingItem] == NO) {
        return;
    }
    
    NSUInteger index = [_items indexOfObject:existingItem];
    [_items insertObject:newItem atIndex:index];
    [self addSubview:newItem.view];
}

- (void)insertItem:(LayoutParams *)newItem afterItem:(LayoutParams *)existingItem {
    if (newItem == nil || [_items containsObject:newItem] == YES || existingItem == nil || [_items containsObject:existingItem] == NO) {
        return;
    }
    
    if (existingItem == [_items lastObject]) {
        [_items addObject:newItem];
    } else {
        NSUInteger index = [_items indexOfObject:existingItem];
        [_items insertObject:newItem atIndex:++index];
    }
    
    [self addSubview:newItem.view];
}

- (void)insertItem:(LayoutParams *)newItem atIndex:(NSUInteger)index {
    if (newItem == nil || [_items containsObject:newItem] == YES || index >= [_items count]) {
        return;
    }
    
    [_items insertObject:newItem atIndex:index];
    [self addSubview:newItem.view];
}

- (void)moveItem:(LayoutParams *)movingItem beforeItem:(LayoutParams *)existingItem {
    if (movingItem == nil || [_items containsObject:movingItem] == NO || existingItem == nil || [_items containsObject:existingItem] == NO || movingItem == existingItem) {
        return;
    }
    
    [_items removeObject:movingItem];
    
    NSUInteger existingItemIndex = [_items indexOfObject:existingItem];
    [_items insertObject:movingItem atIndex:existingItemIndex];
    
    [self setNeedsLayout];
}

- (void)moveItem:(LayoutParams *)movingItem afterItem:(LayoutParams *)existingItem {
    if (movingItem == nil || [_items containsObject:movingItem] == NO || existingItem == nil || [_items containsObject:existingItem] == NO || movingItem == existingItem) {
        return;
    }
    
    [_items removeObject:movingItem];
    
    if (existingItem == [_items lastObject]) {
        [_items addObject:movingItem];
    } else {
        NSUInteger existingItemIndex = [_items indexOfObject:existingItem];
        [_items insertObject:movingItem atIndex:++existingItemIndex];
    }
    
    [self setNeedsLayout];
}

- (void)moveItem:(LayoutParams *)movingItem toIndex:(NSUInteger)index {
    if (movingItem == nil || [_items containsObject:movingItem] == NO || index >= [_items count] || [_items indexOfObject:movingItem] == index) {
        return;
    }
    
    [_items removeObject:movingItem];
    
    if (index == ([_items count] - 1)) {
        [_items addObject:movingItem];
    } else {
        [_items insertObject:movingItem atIndex:index];
    }
    
    [self setNeedsLayout];
}

- (void)swapItem:(LayoutParams *)firstItem withItem:(LayoutParams *)secondItem {
    if (firstItem == nil || [_items containsObject:firstItem] == NO || secondItem == nil || [_items containsObject:secondItem] == NO || firstItem == secondItem) {
        return;
    }
    
    NSUInteger firstItemIndex = [_items indexOfObject:firstItem];
    NSUInteger secondItemIndex = [_items indexOfObject:secondItem];
    [_items exchangeObjectAtIndex:firstItemIndex withObjectAtIndex:secondItemIndex];
    
    [self setNeedsLayout];
}

@end

