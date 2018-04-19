#import <UIKit/UIKit.h>

@interface PaddedLabel : UILabel
- (void)setBounds:(CGRect)bounds;
- (CGSize)intrinsicContentSize;
- (void)relayoutAll;
@property (nonatomic, assign) UIEdgeInsets edgeInsets;

@end
