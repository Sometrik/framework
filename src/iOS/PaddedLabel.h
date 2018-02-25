#import <UIKit/UIKit.h>

@interface PaddedLabel : UILabel
- (void)setBounds:(CGRect)bounds;
- (CGSize)intrinsicContentSize;
@property (nonatomic, assign) UIEdgeInsets edgeInsets;

@end
