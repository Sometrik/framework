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
@property int fixedWidth;
@property int fixedHeight;
@property int weight;
@property NSLayoutConstraint *topConstraint;
@property NSLayoutConstraint *leftConstraint;
@property NSLayoutConstraint *rightConstraint;
@property NSLayoutConstraint *bottomConstraint;
@property NSLayoutConstraint *widthConstraint;
@property NSLayoutConstraint *heightConstraint;
@property NSInteger level;
- (id)initWithView:(UIView *)aView;
+ (LayoutParams *)layoutItemForView:(UIView *)aView;

LinearLayoutItemMargin LinearLayoutMakeMargin(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right);

@end
