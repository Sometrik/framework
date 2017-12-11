#import "FWImageView.h"

@implementation FWImageView

- (void)layoutSubviews {
    [super layoutSubviews];
    if (self.image == nil) {
        NSLog(@"FWImageView, layoutSubview(): %f %f", self.frame.size.width, self.frame.size.height);
        if ([self.delegate respondsToSelector:@selector(fwImageView:didChangeSize:ofImageUrl:)]) {
            [self.delegate fwImageView:self didChangeSize:self.frame.size ofImageUrl:self.url];
        }
    }
}

- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height
{
    self.url = url;
}

- (void)clear
{
    self.image = nil;
    self.url = nil;
}

@end
