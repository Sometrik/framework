#import <UIKit/UIKit.h>

@interface PaddedLabel : UILabel
- (void)setBounds:(CGRect)bounds;
- (CGSize)intrinsicContentSize;
- (void)relayoutAll;
- (NSAttributedString *)createAttributedString:(NSString *)s;
@property (nonatomic, assign) UIEdgeInsets edgeInsets;
@property (nonatomic, assign) BOOL autolink;
@property (nonatomic, strong) UIFont * defaultFont;
@property (nonatomic, strong) UIFont * boldFont;
@property (nonatomic, strong) UIColor * defaultColor;
@end
