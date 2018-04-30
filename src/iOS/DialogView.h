#import <UIKit/UIKit.h>
#import "FrameLayoutView.h"
#import "LayoutParams.h"

@interface DialogView : FrameLayoutView
@property (nonatomic, strong) NSLayoutConstraint *heightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *maxBottomConstraint;
@end
