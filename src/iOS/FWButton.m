#import "FWButton.h"

@implementation FWButton

- (instancetype)init
{
    if (self = [super init]) {
        self.iconAttachment = FWButtonIconAttachmentTop;
	self.translatesAutoresizingMaskIntoConstraints = false;
    }
    return self;
}

- (void)layoutSubviews {
    [super layoutSubviews];

    float padding = 6.0f;

    if (self.imageView.image == nil || self.iconAttachment == FWButtonIconAttachmentLeft) {
        self.titleEdgeInsets = UIEdgeInsetsZero;
        self.imageEdgeInsets = UIEdgeInsetsZero;
        // self.contentEdgeInsets = UIEdgeInsetsZero;
    } else if (self.iconAttachment == FWButtonIconAttachmentRight) {
        self.titleEdgeInsets = UIEdgeInsetsMake(0, -self.imageView.frame.size.width, 0, self.imageView.frame.size.width);
        self.imageEdgeInsets = UIEdgeInsetsMake(0, self.titleLabel.frame.size.width, 0, -self.titleLabel.frame.size.width);
    } else if (self.iconAttachment == FWButtonIconAttachmentBottom || 1) {
        CGSize imageSize = self.imageView.frame.size;
        CGSize titleSize = self.titleLabel.frame.size;
        CGFloat totalHeight = (imageSize.height + titleSize.height + padding);

        self.imageEdgeInsets = UIEdgeInsetsMake(-(totalHeight - imageSize.height),
                                                0.0f,
                                                0.0f,
                                                -titleSize.width);
        
        self.titleEdgeInsets = UIEdgeInsetsMake(0.0f,
                                                -imageSize.width,
                                                -(totalHeight - titleSize.height),
                                                0.0f);
        
        UIEdgeInsets insets = self.contentEdgeInsets;
        self.contentEdgeInsets = UIEdgeInsetsMake(insets.top,
                                                  insets.left,
                                                  titleSize.height,
                                                  insets.right);
    } else {
    
    }
}

@end
