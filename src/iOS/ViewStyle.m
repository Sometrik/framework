#import "ViewStyle.h"

#import "PaddedLabel.h"

@implementation ViewStyle

- (id)init {
    self = [super init];
    if (self) {
        self.alpha = 1.0f;
        self.zoom = 1.0f;
        self.fontSize = 10;
        self.fontWeight = 0;
    }
    return self;
}

- (void)apply:(UIView *)view animate:(BOOL)animate {
    if ([view isKindOfClass:PaddedLabel.class]) {
        PaddedLabel * label = (PaddedLabel*)view;
        label.font = [self createFont:label.font];
    }
    
    if (!animate) {
        view.alpha = self.alpha;
    }
  
    if (animate) {
        [UIView animateWithDuration:0.3 animations:^{
            view.alpha = self.alpha;
        }];
    }
}

- (UIFont *)createFont:(UIFont *)currentFont {
    if (self.fontWeight) {
        UIFontDescriptor * fontD = [currentFont.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
        return [UIFont fontWithDescriptor:fontD size:self.fontSize];
    } else {
        return [currentFont fontWithSize:self.fontSize];
    }
}

@end
