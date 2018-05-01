#import <UIKit/UIKit.h>

@interface FWScrollView : UIScrollView
- (void)layoutSubviews;
- (void)updateVisibility:(CGRect)bounds;
- (void)addChildConstraints:(UIView *)view position:(int)position pageWidth:(int)pageWidth;
- (void)rebuildConstraints:(int)pageWidth;

@property (nonatomic, strong) NSLayoutConstraint *topConstraint;
@property (nonatomic, strong) NSLayoutConstraint *leftConstraint;
@property (nonatomic, strong) NSLayoutConstraint *widthConstraint;
@property (nonatomic, strong) NSLayoutConstraint *heightConstraint;
@end
