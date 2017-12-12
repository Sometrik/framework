#import "FWImageView.h"
#import "FWImage.h"

@interface FWImageView ()
@property (nonatomic, strong) NSMutableArray *images;
@end

@implementation FWImageView

// lazy initialization
- (NSMutableArray *)images
{
    if (!_images) {
        _images = [[NSMutableArray alloc] init];
    }
    return _images;
}

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
    FWImage *newImage = [[FWImage alloc] initWithUrl:url width:width height:height];
    
    // put image to array so it remains sorted by width
    for (int i = 0; i < self.images.count; i++) {
        FWImage *image = (FWImage *)self.images[i];
        if (image.width == newImage.width) { // there already is same image
            break;
        }
        if (image.width > newImage.width) {
            [self.images insertObject:newImage atIndex:i];
            break;
        }
        // if reached end
        if (i == self.images.count - 1) {
            [self.images addObject:newImage];
        }
    }
    if (self.images.count == 0) { // first image in to the array
        [self.images addObject:newImage];
    }
    self.url = url;
}

- (void)clear
{
    self.image = nil;
    self.url = nil;
}

@end
