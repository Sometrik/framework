#import "LinearLayoutView.h"

@interface LinearLayoutView()
- (void)setup;
- (void)adjustFrameSize;
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
    _autoAdjustFrameSize = YES;
    self.autoresizesSubviews = NO;
    self.translatesAutoresizingMaskIntoConstraints = false;
}

#if 0
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
  UIView *hitView = [super hitTest:point withEvent:event];

  // If the hitView is THIS view, return nil and allow hitTest:withEvent: to
  // continue traversing the hierarchy to find the underlying view.
  if (hitView == self) {
    return nil;
  }
  // Else return the hitView (as it could be one of this view's buttons):
  return hitView;
}
#endif

- (void)layoutSubviews {
    
    CGFloat relativePosition = 0.0;
    CGFloat absolutePosition = 0.0;
    
    for (LayoutParams *item in _items) {
        
        CGFloat startPadding = 0.0;
        CGFloat endPadding = 0.0;

        CGFloat width, height;
        if (item.fixedWidth == -1 && self.orientation == LinearLayoutViewOrientationVertical) {
            width = self.frame.size.width - (item.padding.left + item.padding.right);
        } else if (item.fixedWidth > 0) {
            width = item.fixedWidth;
        } else {
            width = [self calcIntrinsicWidth:item.view];
        }
        if (item.fixedHeight == -1 && self.orientation == LinearLayoutViewOrientationVertical) {
            height = self.frame.size.height - (item.padding.top + item.padding.bottom);
        } else if (item.fixedHeight > 0) {
            height = item.fixedHeight;
        } else {
            height = [self calcIntrinsicHeight:item.view];
        }
        
        if (self.orientation == LinearLayoutViewOrientationHorizontal) {            
            startPadding = item.padding.left;
            endPadding = item.padding.right;
            
            if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentTop || item.fixedHeight == -1) {
                absolutePosition = item.padding.top;
            } else if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentBottom) {
                absolutePosition = self.frame.size.height - height - item.padding.bottom;
            } else { // LinearLayoutItemVerticalCenter
                absolutePosition = (self.frame.size.height / 2) - ((height + (item.padding.bottom - item.padding.top)) / 2);
            }            
        } else {           
            startPadding = item.padding.top;
            endPadding = item.padding.bottom;
            
            if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentLeft || item.fixedWidth == -1) {
                absolutePosition = item.padding.left;
            } else if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentRight) {
                absolutePosition = self.frame.size.width - width - item.padding.right;
            } else { // LinearLayoutItemHorizontalCenter
                absolutePosition = (self.frame.size.width / 2) - ((width + (item.padding.right - item.padding.left)) / 2);
            }            
        }
        
        relativePosition += startPadding; 
       
        CGFloat currentOffset = 0.0;
        if (self.orientation == LinearLayoutViewOrientationHorizontal) {
            item.leftConstraint.constant = relativePosition;
            item.topConstraint.constant = absolutePosition;
            item.rightConstraint.constant = relativePosition + width;
            item.bottomConstraint.constant = absolutePosition + height;
            currentOffset = width;
        } else {
            item.leftConstraint.constant = absolutePosition;
            item.topConstraint.constant = relativePosition;
            item.rightConstraint.constant = absolutePosition + width;
            item.bottomConstraint.constant = relativePosition + height;
            currentOffset = height;
        }

        [item.view setNeedsLayout];
        [item.view needsUpdateConstraints];
        [item.view setNeedsDisplay];
        
        relativePosition += currentOffset + endPadding;
    }
        
    if (_autoAdjustFrameSize == YES) {
        [self adjustFrameSize];
    }
}

- (void)adjustFrameSize {
#if 0
    int width = [self calcIntrinsicWidth:self];
    int height = [self calcIntrinsicHeight:self];
    self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, width, height);
#else
    if (self.orientation == LinearLayoutViewOrientationHorizontal) {
        self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, self.layoutOffset, self.frame.size.height);
    } else {
        self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, self.frame.size.width, self.layoutOffset);
    }
#endif
}

- (CGFloat)layoutOffset {
    CGFloat currentOffset = 0.0;
    
    for (LayoutParams *item in _items) {
        if (_orientation == LinearLayoutViewOrientationHorizontal) {
            currentOffset += item.padding.left + item.view.frame.size.width + item.padding.right;
        } else {
            currentOffset += item.padding.top + item.view.frame.size.height + item.padding.bottom;
        }
    }
    
    return currentOffset;
}

- (void)setOrientation:(LinearLayoutViewOrientation)anOrientation {
    _orientation = anOrientation;
    [self setNeedsLayout];
}

- (void)addSubview:(UIView *)view {
    [super addSubview:view];
    
    if (_autoAdjustFrameSize == YES) {
        [self adjustFrameSize];
    }
}

- (int)calcIntrinsicWidth:(UIView *)view {
    if ([view isKindOfClass:LinearLayoutView.class]) {
        LinearLayoutView * child = (LinearLayoutView*)view;
        if (child.orientation == LinearLayoutViewOrientationHorizontal) {
            return child.layoutOffset;
        } else {
            int width = 0;
            for (LayoutParams *item in child.items) {
                int w;
                if (item.fixedWidth == -1) {
                    w = item.view.superview.frame.size.width;
                } else if (item.fixedWidth > 0) {
                    w = item.fixedWidth;
                } else {
                    w = [self calcIntrinsicWidth:item.view];
                }
                if (w > width) width = w;
            }
            return view.layoutMargins.left + view.layoutMargins.right + width;
        }
    } else {
        NSUInteger c = [[view subviews] count];
        if (c >= 1) {
            UIView * subview = [[view subviews] objectAtIndex:0];
            return view.layoutMargins.left + view.layoutMargins.right + [self calcIntrinsicWidth:subview];
        } else {
            return view.intrinsicContentSize.width;
        }
    }
}

- (int)calcIntrinsicHeight:(UIView *)view {
    if ([view isKindOfClass:LinearLayoutView.class]) {
        LinearLayoutView * child = (LinearLayoutView*)view;
        if (child.orientation == LinearLayoutViewOrientationVertical) {
            return child.layoutOffset;
        } else {
            int height = 0;
            for (LayoutParams *item in child.items) {
                int h;
                if (item.fixedHeight == -1) {
                    h = item.view.superview.frame.size.height;
                } else if (item.fixedHeight > 0) {
                    h = item.fixedHeight;
                } else {
                    h = [self calcIntrinsicHeight:item.view];
                }
                if (h > height) height = h;
            }
            return view.layoutMargins.top + view.layoutMargins.bottom + height;
        }
    } else {
        NSUInteger c = [[view subviews] count];
        if (c >= 1) {
            UIView * subview = [[view subviews] objectAtIndex:0];
            return view.layoutMargins.top + view.layoutMargins.bottom + [self calcIntrinsicHeight:subview];
        } else {
            return view.intrinsicContentSize.height;
        }
    }
}

- (void)addItem:(LayoutParams *)linearLayoutItem {
    if (linearLayoutItem == nil || [_items containsObject:linearLayoutItem] == YES || linearLayoutItem.view == nil) {
        return;
    }
    
    [_items addObject:linearLayoutItem];
    [self addSubview:linearLayoutItem.view];

    linearLayoutItem.topConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    linearLayoutItem.leftConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    linearLayoutItem.rightConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    linearLayoutItem.bottomConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];

    linearLayoutItem.topConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.leftConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.rightConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.bottomConstraint.priority = 999 - linearLayoutItem.level;

    [self addConstraints:@[linearLayoutItem.topConstraint, linearLayoutItem.leftConstraint]];
    [self addConstraints:@[linearLayoutItem.rightConstraint, linearLayoutItem.bottomConstraint]];
}

- (void)removeItem:(LayoutParams *)linearLayoutItem {
    if (linearLayoutItem == nil || [_items containsObject:linearLayoutItem] == NO) {
        return;
    }
    
    [linearLayoutItem.view removeFromSuperview];
    [_items removeObject:linearLayoutItem];
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

