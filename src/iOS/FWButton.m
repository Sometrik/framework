#import "FWButton.h"

@implementation FWButton

- (instancetype)init
{
    if (self = [super init]) {
        self.iconAttachment = FWButtonIconAttachmentLeft;
    }
    return self;
}

- (void)layoutSubviews {
    [super layoutSubviews];
    if (self.iconAttachment == FWButtonIconAttachmentRight) {
        self.titleEdgeInsets = UIEdgeInsetsMake(0, -self.imageView.frame.size.width, 0, self.imageView.frame.size.width);
        self.imageEdgeInsets = UIEdgeInsetsMake(0, self.titleLabel.frame.size.width, 0, -self.titleLabel.frame.size.width);
    }
}

@end
