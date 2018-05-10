#import "FWScrollView.h"

#import "FrameLayoutView.h"
#import "LinearLayoutView.h"

@implementation FWScrollView

- (instancetype)init
{
    if (self = [super init]) {
        self.topConstraint = nil;
        self.leftConstraint = nil;
        self.widthConstraint = nil;
        self.heightConstraint = nil;
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
        self.topConstraint = nil;
        self.leftConstraint = nil;
        self.widthConstraint = nil;
        self.heightConstraint = nil;
        self.translatesAutoresizingMaskIntoConstraints = false;
        self.currentPage = 0;
	self.currentPageInternalId = 0;
	self.items = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)layoutSubviews {
    if (self.pagingEnabled) {
        [self updateChildConstraints];
    } else {
        int width = self.frame.size.width;
        int height = 0;

        for (UIView * view in [self subviews]) {
            if ([view isKindOfClass:UIImageView.class]) continue; // ignore scroll indicators
            height = [self calcIntrinsicHeight:view];
            break;
        }

        self.widthConstraint.constant = width;
        self.heightConstraint.constant = height;

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
        for (UIView * subview in [view subviews]) {
	    if ([subview isKindOfClass:UIImageView.class]) continue; // ignore scroll indicators
	    return [self calcIntrinsicHeight:subview];
	}
	return 0;
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

    item.leftConstraint.constant = numChildren * self.frame.size.width;
    item.widthConstraint.constant = self.frame.size.width;
    item.heightConstraint.constant = self.frame.size.height;

    item.topConstraint.active = YES;
    item.leftConstraint.active = YES;
    item.widthConstraint.active = YES;
    item.heightConstraint.active = YES;
       
    self.contentSize = CGSizeMake(numChildren * self.frame.size.width, self.frame.size.height);

    [self setNeedsLayout];
}

- (void)removeItem:(LayoutParams *)item {
    if (item == nil || [self.items containsObject:item] == NO) {
        return;
    }
    
    [item.view removeFromSuperview];
    [self.items removeObject:item];

    [self updateChildConstraints];
    [self setNeedsLayout];
}

- (void)removeAllItems {
    // only remove actual items, not scrollbars
    for (LayoutParams *item in self.items) {
        [item.view removeFromSuperview];
    }
    [self.items removeAllObjects];

    [self updateChildConstraints];
    [self setNeedsLayout];
}

- (void)insertItem:(LayoutParams *)newItem beforeItem:(LayoutParams *)existingItem {
    if (newItem == nil || [self.items containsObject:newItem] == YES || existingItem == nil ||  [self.items containsObject:existingItem] == NO) {
        return;
    }
    
    NSUInteger index = [self.items indexOfObject:existingItem];
    [self.items insertObject:newItem atIndex:index];
    [self addSubview:newItem.view];

    [self updateChildConstraints];
    [self setNeedsLayout];
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

    [self updateChildConstraints];
    [self setNeedsLayout];
}

- (void)insertItem:(LayoutParams *)newItem atIndex:(NSUInteger)index {
    if (newItem == nil || [self.items containsObject:newItem] == YES || index >= [self.items count]) {
        return;
    }
    
    [self.items insertObject:newItem atIndex:index];
    [self addSubview:newItem.view];

    [self updateChildConstraints];
    [self setNeedsLayout];
}

- (void)moveItem:(LayoutParams *)movingItem beforeItem:(LayoutParams *)existingItem {
    if (movingItem == nil || [self.items containsObject:movingItem] == NO || existingItem == nil || [self.items containsObject:existingItem] == NO || movingItem == existingItem) {
        return;
    }
    
    [self.items removeObject:movingItem];
    
    NSUInteger existingItemIndex = [self.items indexOfObject:existingItem];
    [self.items insertObject:movingItem atIndex:existingItemIndex];
    
    [self updateChildConstraints];
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
    
    [self updateChildConstraints];
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
    
    [self updateChildConstraints];
    [self setNeedsLayout];
}

- (void)swapItem:(LayoutParams *)firstItem withItem:(LayoutParams *)secondItem {
    if (firstItem == nil || [self.items containsObject:firstItem] == NO || secondItem == nil || [self.items containsObject:secondItem] == NO || firstItem == secondItem) {
        return;
    }
    
    NSUInteger firstItemIndex = [self.items indexOfObject:firstItem];
    NSUInteger secondItemIndex = [self.items indexOfObject:secondItem];
    [self.items exchangeObjectAtIndex:firstItemIndex withObjectAtIndex:secondItemIndex];

    [self updateChildConstraints];
    [self setNeedsLayout];
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
            
        item.leftConstraint.constant = numChildren * self.frame.size.width;
        item.widthConstraint.constant = self.frame.size.width;
        item.heightConstraint.constant = self.frame.size.height;
	    
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

@end
