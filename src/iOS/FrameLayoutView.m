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
    
    for (LayoutParams *item in _items) {
      if (item.view.hidden) continue;

        CGFloat width, height;
        if (item.fixedWidth == -1) {
            width = self.frame.size.width - (item.margin.left + item.margin.right + self.layoutMargins.left + self.layoutMargins.right);
        } else if (item.fixedWidth > 0) {
            width = item.fixedWidth;
        } else {
	    width = [self calcIntrinsicWidth:item.view];
        }
        if (item.fixedHeight == -1) {
            height = self.frame.size.height - (item.margin.top + item.margin.bottom + self.layoutMargins.top + self.layoutMargins.bottom);
        } else if (item.fixedHeight > 0) {
            height = item.fixedHeight;
        } else {
	    height = [self calcIntrinsicHeight:item.view];
        }

	if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentBottom) {
	  item.topConstraint.constant = self.frame.size.height - item.margin.bottom - height;
	  item.bottomConstraint.constant = self.frame.size.height - item.margin.bottom;
	} else {
	  item.topConstraint.constant = item.margin.top;
	  item.bottomConstraint.constant = item.margin.top + height;
        }
              
        item.leftConstraint.constant = item.margin.left;
        item.rightConstraint.constant = item.margin.left + width;
       
        [item.view setNeedsLayout];
        [item.view needsUpdateConstraints];
        [item.view setNeedsDisplay];
    }
}

- (int)calcIntrinsicWidth:(UIView *)view {
    if ([view isKindOfClass:LinearLayoutView.class]) {
        LinearLayoutView * child = (LinearLayoutView*)view;
        if (child.orientation == LinearLayoutViewOrientationHorizontal) {
            return view.layoutMargins.left + view.layoutMargins.right + child.layoutOffset;
        } else {
            int width = 0;
            for (LayoutParams *item in child.items) {
	        if (item.view.hidden) continue;

                int w = 0;
		if (item.fixedWidth > 0) {
                    w = item.fixedWidth + item.margin.left + item.margin.right;
                } else {
                    w = [self calcIntrinsicWidth:item.view] + item.margin.left + item.margin.right;
                }
                if (w > width) width = w;
            }
            return view.layoutMargins.left + view.layoutMargins.right + width;
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
	  w = [self calcIntrinsicWidth:item.view] + item.margin.left + item.margin.right;
	}
	if (w > width) width = w;
      }
      return view.layoutMargins.left + view.layoutMargins.right + width;          
    } else {
      return view.intrinsicContentSize.width;
    }
}

- (int)calcIntrinsicHeight:(UIView *)view {
    if ([view isKindOfClass:LinearLayoutView.class]) {
        LinearLayoutView * child = (LinearLayoutView*)view;
        if (child.orientation == LinearLayoutViewOrientationVertical) {
            return view.layoutMargins.top + view.layoutMargins.bottom + child.layoutOffset;
        } else {
            int height = 0;
            for (LayoutParams *item in child.items) {
	        if (item.view.hidden) continue;

                int h = 0;
		if (item.fixedHeight > 0) {
                    h = item.fixedHeight + item.margin.top + item.margin.bottom;
                } else {
                    h = [self calcIntrinsicHeight:item.view] + item.margin.top + item.margin.bottom;
                }
                if (h > height) height = h;
            }
            return view.layoutMargins.top + view.layoutMargins.bottom + height;
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
	  h = [self calcIntrinsicHeight:item.view] + item.margin.top + item.margin.bottom;
	}
	if (h > height) height = h;
      }
      return view.layoutMargins.top + view.layoutMargins.bottom + height;
    } else {
      return view.intrinsicContentSize.height;
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

