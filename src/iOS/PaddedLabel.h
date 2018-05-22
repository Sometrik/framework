#import <UIKit/UIKit.h>
@class PaddedLabel;

@protocol PaddedLabelDelegate <NSObject>
- (void)paddedLabel:(PaddedLabel *)label didOpenLinkURL:(NSURL *)url;
@end

@interface PaddedLabel : UILabel
- (void)setBounds:(CGRect)bounds;
- (CGSize)intrinsicContentSize;
- (void)relayoutAll;
- (NSAttributedString *)createAttributedString:(NSString *)s;

@property (nonatomic, assign) UIEdgeInsets edgeInsets;
@property (nonatomic, assign) BOOL autolink;
@property (nonatomic, assign) BOOL markdown;
@property (nonatomic, strong) UIFont * defaultFont;
@property (nonatomic, strong) UIFont * boldFont;
@property (nonatomic, strong) UIColor * defaultColor;
@property (nonatomic, strong) NSString * origText;
@property (nonatomic, assign) id<PaddedLabelDelegate> delegate;
@end
