#import "FWScrollView.h"

#import "FrameLayoutView.h"
#import "LinearLayoutView.h"

@implementation FWScrollView

- (instancetype)init
{
    if (self = [super init]) {
        self.translatesAutoresizingMaskIntoConstraints = false;
        self.currentPage = 0;
	self.currentPageInternalId = 0;
	self.items = [[NSMutableArray alloc] init];
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        self.frame = frame;
        self.translatesAutoresizingMaskIntoConstraints = false;
        self.currentPage = 0;
	self.currentPageInternalId = 0;
	self.items = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)adjustedContentInsetDidChange {
    [super adjustedContentInsetDidChange];
    NSLog(@"AdjustedContentInsetDidChange top %f offset %f", self.adjustedContentInset.top, self.contentOffset.y);
    self.contentOffset = CGPointMake(0, -self.adjustedContentInset.top);
}

- (void)layoutSubviews {
    if (self.pagingEnabled) {
        [self updateChildConstraints];
    } else {
        int width = self.frame.size.width;
        int height = self.superview.frame.size.height + 1;
        
        if ([self.items count] > 0) {
            LayoutParams * item = [self.items firstObject];
            int itemHeight = [self calcIntrinsicHeight:item.view] + item.margin.top + item.margin.bottom + item.padding.top + item.padding.bottom;
            if (itemHeight > height) {
                height = itemHeight;
            }

            item.topConstraint.constant = item.margin.top;
            item.leftConstraint.constant = item.margin.left;
            item.widthConstraint.constant = width - item.margin.left - item.margin.right;
            item.heightConstraint.constant = height - item.margin.top - item.margin.bottom;
        }

        self.contentSize = CGSizeMake(width, height);
    }

    [super layoutSubviews];
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
    } else if ([view isKindOfClass:FWScrollView.class]) {
        FWScrollView * scrollView = (FWScrollView *)view;
	int height = 0;
        if (scrollView.pagingEnabled) {
	    for (LayoutParams * item in scrollView.items) {
	        if (item.view.hidden) continue;

                int h = 0;
                if (item.fixedHeight > 0) {
                    h = item.fixedHeight + item.margin.top + item.margin.bottom;
                } else {
                    h = [self calcIntrinsicHeight:item.view] + item.padding.top + item.padding.bottom + item.margin.top + item.margin.bottom;
                }
                if (h > height) height = h;
	    }
	} else {
	    for (UIView * subview in [view subviews]) {
	      if ([subview isKindOfClass:UIImageView.class]) continue; // ignore scroll indicators
	      height = [self calcIntrinsicHeight:subview];
	      break;
	    }
        }
	return height;
    } else {
        return view.intrinsicContentSize.height;
    }
}

- (void)updateVisibility:(CGRect)bounds
{
    for (UIView * subview in [self subviews]) {
        if ([subview isKindOfClass:FWLayoutView.class]) {
            FWLayoutView * layout = (FWLayoutView*)subview;
            [layout updateVisibility:bounds];
        }
    }
}

- (void)flush {
    [self updateChildConstraints];
    [self setNeedsLayout];
}

- (void)addItem:(LayoutParams *)item {
    if (item == nil || [self.items containsObject:item] == YES || item.view == nil) {
        return;
    }

    int numChildren = 0;
    for (LayoutParams *item in self.items) {
        if (item.view.hidden) continue;
        numChildren++;
    }
    
    [self.items addObject:item];

    [self addSubview:item.view];

    item.topConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    item.leftConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:self attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    item.widthConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:0];
    item.heightConstraint = [NSLayoutConstraint constraintWithItem:item.view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:0];

    item.topConstraint.priority = 999 - item.level;
    item.leftConstraint.priority = 999 - item.level;
    item.widthConstraint.priority = 999 - item.level - 1;
    item.heightConstraint.priority = 999 - item.level - 1;

    item.topConstraint.active = YES;
    item.leftConstraint.active = YES;
    item.widthConstraint.active = YES;
    item.heightConstraint.active = YES;
    
    [self setNeedsLayout];
}

- (void)removeItem:(LayoutParams *)item {
    if (item == nil || [self.items containsObject:item] == NO) {
        return;
    }
    
    [item.view removeFromSuperview];
    [self.items removeObject:item];
}

- (void)removeAllItems {
    // only remove actual items, not scrollbars
    for (LayoutParams *item in self.items) {
        [item.view removeFromSuperview];
    }
    [self.items removeAllObjects];
}

- (void)insertItem:(LayoutParams *)newItem beforeItem:(LayoutParams *)existingItem {
    if (newItem == nil || [self.items containsObject:newItem] == YES || existingItem == nil ||  [self.items containsObject:existingItem] == NO) {
        return;
    }
    
    NSUInteger index = [self.items indexOfObject:existingItem];
    [self.items insertObject:newItem atIndex:index];
    [self addSubview:newItem.view];
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
}

- (void)insertItem:(LayoutParams *)newItem atIndex:(NSUInteger)index {
    if (newItem == nil || [self.items containsObject:newItem] == YES || index >= [self.items count]) {
        return;
    }
    
    [self.items insertObject:newItem atIndex:index];
    [self addSubview:newItem.view];
}

- (void)moveItem:(LayoutParams *)movingItem toIndex:(NSUInteger)index {
    if (movingItem == nil || [self.items containsObject:movingItem] == NO || [self.items indexOfObject:movingItem] == index) {
        return;
    }
    
    [self.items removeObject:movingItem];
    
    if (index >= [self.items count]) {
        [self.items addObject:movingItem];
    } else {
        [self.items insertObject:movingItem atIndex:index];
    }
}

- (void)swapItem:(LayoutParams *)firstItem withItem:(LayoutParams *)secondItem {
    if (firstItem == nil || [self.items containsObject:firstItem] == NO || secondItem == nil || [self.items containsObject:secondItem] == NO || firstItem == secondItem) {
        return;
    }
    
    NSUInteger firstItemIndex = [self.items indexOfObject:firstItem];
    NSUInteger secondItemIndex = [self.items indexOfObject:secondItem];
    [self.items exchangeObjectAtIndex:firstItemIndex withObjectAtIndex:secondItemIndex];
}

- (NSInteger)indexForVisiblePage
{
    CGRect frame = self.bounds;
    return (frame.origin.x + frame.size.width / 2) / frame.size.width;
}

- (void)updateChildConstraints
{
    int numChildren = 0;
    for (LayoutParams *item in self.items) {
        if (item.view.hidden) continue;
            
	item.topConstraint.constant = item.margin.top;
        item.leftConstraint.constant = numChildren * self.frame.size.width + item.margin.left;
        item.widthConstraint.constant = self.frame.size.width - item.margin.left - item.margin.right;
        item.heightConstraint.constant = self.frame.size.height - item.margin.top - item.margin.bottom;;
	    
        numChildren++;
    }
    self.contentSize = CGSizeMake(numChildren * self.frame.size.width, self.frame.size.height);
}

- (void)setPage:(NSInteger)page
{
    self.currentPage = page;
    self.currentPageInternalId = 0;
    int i = 0;
    for (LayoutParams *item in self.items) {
        if (item.view.hidden) continue;
	if (i == page) {
	    self.currentPageInternalId = item.view.tag;
	    break;
	}
	i++;
    }
}

- (BOOL)reselectCurrentPage {
    int i = 0;
    for (LayoutParams *item in self.items) {
        if (item.view.hidden) continue;
	if (item.view.tag == self.currentPageInternalId) {
	    if (self.currentPage != i) {
               [self showPage:i animated:NO];
	       return YES;
            } else {
	       return NO;
	    }
	}
	i++;
    }
    return NO;
}

- (void)showPage:(NSInteger)page animated:(BOOL)animated
{
    [self setPage:page];
    CGRect frame = self.frame;
    frame.origin.x = frame.size.width * page;
    frame.origin.y = 0;
    [self scrollRectToVisible:frame animated:animated];
}

- (LayoutParams*)findParams:(UIView *)view {
    for (LayoutParams *item in self.items) {
        if (item.view == view) {
            return item;
        }
    }
    return nil;
}

@end
