#import <UIKit/UIKit.h>

typedef enum {
    LinearLayoutItemFillModeNormal,
    LinearLayoutItemFillModeStretch
} LinearLayoutItemFillMode;

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
} LinearLayoutItemPadding;

@interface LayoutParams : NSObject

@property (nonatomic, strong) UIView *view;
@property (nonatomic, assign) LinearLayoutItemFillMode fillMode;
@property (nonatomic, assign) LinearLayoutItemHorizontalAlignment horizontalAlignment;
@property (nonatomic, assign) LinearLayoutItemVerticalAlignment verticalAlignment;
@property (nonatomic, assign) LinearLayoutItemPadding padding;
@property int fixedWidth;
@property int fixedHeight;
@property NSLayoutConstraint *topConstraint;
@property NSLayoutConstraint *leftConstraint;
@property NSLayoutConstraint *rightConstraint;
@property NSLayoutConstraint *bottomConstraint;
@property NSLayoutConstraint *widthConstraint;
@property NSLayoutConstraint *heightConstraint;
@property NSInteger level;
- (id)initWithView:(UIView *)aView;
+ (LayoutParams *)layoutItemForView:(UIView *)aView;

LinearLayoutItemPadding LinearLayoutMakePadding(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right);

@end
