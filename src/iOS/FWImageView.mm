#import "FWImageView.h"

@implementation FWImageView

- (void)layoutSubviews {
    [super layoutSubviews];
    NSLog(@"FWImageView, layoutSubview(): %f %f", self.frame.size.width, self.frame.size.height);
}

- (void)addImageUrl: (int)viewId width:(int)width height:(int)height url:(NSString *)url
{
    self.url = url;
}

@end
