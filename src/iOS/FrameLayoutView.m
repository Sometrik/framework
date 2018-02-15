#import "FrameLayoutView.h"

#import "LinearLayoutView.h"

@interface FrameLayoutView()
- (void)setup;
@end

@implementation FrameLayoutView

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

    for (LayoutParams *item in _items) {
      if (item.view.hidden) continue;

        if (item.fixedWidth == -1) {
	    item.leftConstraint.constant = paddingLeft + item.margin.left;
	    item.rightConstraint.constant = -(paddingRight + item.margin.right);

	    item.leftConstraint.active = YES;
	    item.rightConstraint.active = YES;
	    item.centerXConstraint.active = NO;
	    item.widthConstraint.active = NO;	    
        } else {
	  CGFloat width;
	  if (item.fixedWidth > 0) {
            width = item.fixedWidth;
	  } else {
	    width = [self calcIntrinsicWidth:item.view] + item.padding.left + item.padding.right;
	  }

	  item.widthConstraint.constant = width;
	  item.widthConstraint.active = YES;

	  if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentRight) {
	    item.rightConstraint.constant = -(paddingRight + item.margin.left);

	    item.leftConstraint.active = NO;
	    item.rightConstraint.active = YES;
	    item.centerXConstraint.active = NO;
	  } else if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentLeft) {
	    item.leftConstraint.constant = paddingLeft + item.margin.left;

	    item.leftConstraint.active = YES;
	    item.rightConstraint.active = NO;
	    item.centerXConstraint.active = NO;
	  } else {
	    item.leftConstraint.active = NO;
	    item.rightConstraint.active = NO;
	    item.centerXConstraint.active = YES;	    
	  }
	}

        if (item.fixedHeight == -1) {
	    item.topConstraint.constant = paddingTop + item.margin.top;
	    item.bottomConstraint.constant = -(paddingBottom + item.margin.bottom);

	    item.topConstraint.active = YES;
	    item.bottomConstraint.active = YES;
	    item.centerYConstraint.active = NO;
	    item.heightConstraint.active = NO;
        } else {
	  CGFloat height;
	  if (item.fixedHeight > 0) {
            height = item.fixedHeight;
	  } else {
	    height = [self calcIntrinsicHeight:item.view] + item.padding.left + item.padding.right;
	  }

	  item.heightConstraint.constant = height;
	  item.heightConstraint.active = YES;

	  if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentBottom) {
	    item.bottomConstraint.constant = -(paddingBottom + item.margin.bottom);
	    item.topConstraint.active = NO;
	    item.bottomConstraint.active = YES;
	    item.centerYConstraint.active = NO;
	  } else if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentTop) {
	    item.topConstraint.constant = paddingTop + item.margin.top;
	    item.topConstraint.active = YES;
	    item.bottomConstraint.active = NO;
	    item.centerYConstraint.active = NO;
	  } else {
	    item.topConstraint.active = NO;
	    item.bottomConstraint.active = NO;
	    item.centerYConstraint.active = YES;
	  }
	}

#if 0                     
        [item.view setNeedsLayout];
        [item.view needsUpdateConstraints];
        [item.view setNeedsDisplay];
#endif
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
        }
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
        if (child.orientation == LinearLayoutViewOrientationVertical) {
            return child.layoutOffset;
        } else {
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
        }
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

- (void)addItem:(LayoutParams *)linearLayoutItem {
    if (linearLayoutItem == nil || [_items containsObject:linearLayoutItem] == YES || linearLayoutItem.view == nil) {
        return;
    }
    
    [_items addObject:linearLayoutItem];
    [self addSubview:linearLayoutItem.view];

    linearLayoutItem.topConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    linearLayoutItem.leftConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    linearLayoutItem.rightConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    linearLayoutItem.bottomConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    linearLayoutItem.centerXConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeCenterX relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeCenterX multiplier:1.f constant:0.0f];
    linearLayoutItem.centerYConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeCenterY relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeCenterY multiplier:1.f constant:0.0f];
    linearLayoutItem.widthConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:0];
    linearLayoutItem.heightConstraint = [NSLayoutConstraint constraintWithItem:linearLayoutItem.view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:0];

    linearLayoutItem.topConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.leftConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.rightConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.bottomConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.centerXConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.centerYConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.widthConstraint.priority = 999 - linearLayoutItem.level;
    linearLayoutItem.heightConstraint.priority = 999 - linearLayoutItem.level;
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

