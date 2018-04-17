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
    self.items = [[NSMutableArray alloc] init];
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

    for (LayoutParams *item in self.items) {
        if (item.view.hidden) continue;

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
	    item.maxHeightConstraint.active = NO;
        } else {
	    item.maxHeightConstraint.constant = -(paddingTop + item.margin.top + paddingBottom + item.margin.bottom);
	    item.maxHeightConstraint.active = YES;

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
    }

    for (CALayer * layer in self.layer.sublayers) {
      layer.frame = self.bounds;
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

