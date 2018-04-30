#import <UIKit/UIKit.h>

typedef enum {
    LinearLayoutItemHorizontalAlignmentLeft,
    LinearLayoutItemHorizontalAlignmentRight,
    LinearLayoutItemHorizontalAlignmentCenter
} LinearLayoutItemHorizontalAlignment;

typedef enum {
    LinearLayoutItemVerticalAlignmentTop,
    LinearLayoutItemVerticalAlignmentBottom,
    LinearLayoutItemVerticalAlignmentCenter
} LinearLayoutItemVerticalAlignment;      

typedef struct {
    CGFloat top;
    CGFloat left;
    CGFloat bottom;
    CGFloat right;
} LinearLayoutItemMargin;

@interface LayoutParams : NSObject

@property (nonatomic, strong) UIView *view;
@property (nonatomic, assign) LinearLayoutItemHorizontalAlignment horizontalAlignment;
@property (nonatomic, assign) LinearLayoutItemVerticalAlignment verticalAlignment;
@property (nonatomic, assign) LinearLayoutItemMargin margin;
@property (nonatomic, assign) LinearLayoutItemMargin padding;
@property (nonatomic, assign) int fixedWidth;
@property (nonatomic, assign) int fixedHeight;
@property (nonatomic, assign) int weight;
@property (nonatomic, strong) NSLayoutConstraint *topConstraint;
@property (nonatomic, strong) NSLayoutConstraint *leftConstraint;
@property (nonatomic, strong) NSLayoutConstraint *rightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *bottomConstraint;
@property (nonatomic, strong) NSLayoutConstraint *centerXConstraint;
@property (nonatomic, strong) NSLayoutConstraint *centerYConstraint;
@property (nonatomic, strong) NSLayoutConstraint *widthConstraint;
@property (nonatomic, strong) NSLayoutConstraint *heightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *minWidthConstraint;
@property (nonatomic, strong) NSLayoutConstraint *minHeightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *maxWidthConstraint;
@property (nonatomic, strong) NSLayoutConstraint *maxHeightConstraint;
@property (nonatomic, assign) NSInteger level;

- (id)initWithView:(UIView *)aView;
+ (LayoutParams *)layoutItemForView:(UIView *)aView;

LinearLayoutItemMargin LinearLayoutMakeMargin(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right);

@end
