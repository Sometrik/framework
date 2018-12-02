#import "FrameLayoutView.h"

@interface DialogView : FrameLayoutView
@property (nonatomic, strong) NSLayoutConstraint *leftConstraint;
@property (nonatomic, strong) NSLayoutConstraint *rightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *heightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *maxHeightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *centerYConstraint;
@end
