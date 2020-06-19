#import "LayoutParams.h"

LinearLayoutItemMargin LLMMargin(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right) {
    LinearLayoutItemMargin margin;
    margin.top = top;
    margin.left = left;
    margin.bottom = bottom;
    margin.right = right;
    
    return margin;
}

@implementation LayoutParams

- (id)init {
    self = [super init];
    if (self) {
        self.horizontalAlignment = LinearLayoutItemHorizontalAlignmentLeft;
        self.verticalAlignment = LinearLayoutItemVerticalAlignmentTop;
	self.topConstraint = nil;
	self.leftConstraint = nil;
	self.rightConstraint = nil;
	self.bottomConstraint = nil;
	self.centerXConstraint = nil;
	self.centerYConstraint = nil;
	self.widthConstraint = nil;
	self.heightConstraint = nil;
	self.minWidthConstraint = nil;
	self.minHeightConstraint = nil;
	self.maxWidthConstraint = nil;
	self.maxHeightConstraint = nil;
	self.margin = LLMMargin(0, 0, 0, 0);
	self.padding = LLMMargin(0, 0, 0, 0);
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
	self.rightConstraint = nil;
	self.bottomConstraint = nil;
	self.centerXConstraint = nil;
	self.centerYConstraint = nil;
	self.widthConstraint = nil;
	self.heightConstraint = nil;
	self.minWidthConstraint = nil;
	self.minHeightConstraint = nil;
	self.maxWidthConstraint = nil;
	self.maxHeightConstraint = nil;
	self.margin = LLMMargin(0, 0, 0, 0);
	self.padding = LLMMargin(0, 0, 0, 0);
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

@end



