#import <UIKit/UIKit.h>

typedef enum {
    FWButtonIconAttachmentLeft,
    FWButtonIconAttachmentRight,
    FWButtonIconAttachmentTop,
    FWButtonIconAttachmentBottom
} FWButtonIconAttachment;

@interface FWButton : UIButton
@property (nonatomic, assign) FWButtonIconAttachment iconAttachment;
- (void)layoutSubviews;
@end
