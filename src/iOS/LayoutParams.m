#import "LayoutParams.h"

@implementation LayoutParams

- (id)init {
    self = [super init];
    if (self) {
        self.horizontalAlignment = LinearLayoutItemHorizontalAlignmentLeft;
        self.verticalAlignment = LinearLayoutItemVerticalAlignmentTop;
	self.topConstraint = nil;
	self.leftConstraint = nil;
	self.widthConstraint = nil;
	self.heightConstraint = nil;
	self.padding = LinearLayoutMakePadding(0, 0, 0, 0);
	self.level = 0;
	self.fixedWidth = 0;
	self.fixedHeight = 0;
	self.weight = 0;
    }
    return self;
}

- (id)initWithView:(UIView *)aView {
    self = [super init];
    if (self) {
        self.view = aView;
        self.horizontalAlignment = LinearLayoutItemHorizontalAlignmentLeft;
        self.verticalAlignment = LinearLayoutItemVerticalAlignmentTop;
	self.topConstraint = nil;
	self.leftConstraint = nil;
	self.widthConstraint = nil;
	self.heightConstraint = nil;
	self.padding = LinearLayoutMakePadding(0, 0, 0, 0);
	self.level = 0;
	self.fixedWidth = 0;
	self.fixedHeight = 0;
	self.weight = 0;
    }
    return self;
}

+ (LayoutParams *)layoutItemForView:(UIView *)aView {
    LayoutParams *item = [[LayoutParams alloc] initWithView:aView];
    return item;
}

LinearLayoutItemPadding LinearLayoutMakePadding(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right) {
    LinearLayoutItemPadding padding;
    padding.top = top;
    padding.left = left;
    padding.bottom = bottom;
    padding.right = right;
    
    return padding;
}

@end



