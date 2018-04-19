#import <UIKit/UIKit.h>

@interface PaddedLabel : UILabel
- (void)setBounds:(CGRect)bounds;
- (CGSize)intrinsicContentSize;
- (void)relayoutAll;
- (NSAttributedString *)createAttributedString:(NSString *)s;
@property (nonatomic, assign) UIEdgeInsets edgeInsets;

@end
