#import "FrameLayoutView.h"

@interface FrameView : FrameLayoutView
@property (nonatomic, strong) NSLayoutConstraint *leftConstraint;
@property (nonatomic, strong) NSLayoutConstraint *rightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *topConstraint;
@property (nonatomic, strong) NSLayoutConstraint *bottomConstraint;
@end
