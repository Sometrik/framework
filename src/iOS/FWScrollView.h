#import <UIKit/UIKit.h>

@interface FWScrollView : UIScrollView
- (void)layoutSubviews;
@property NSLayoutConstraint *topConstraint;
@property NSLayoutConstraint *leftConstraint;
@property NSLayoutConstraint *widthConstraint;
@property NSLayoutConstraint *heightConstraint;
@end
