#import "FWImageView.h"
#import "FWImage.h"

@interface FWImageView ()
@property (nonatomic, strong) NSMutableArray *images;
@end

@implementation FWImageView

- (instancetype)init
{
    if (self = [super init]) {
        self.prevWidth = 0;
        self.prevHeight = 0;
        self.translatesAutoresizingMaskIntoConstraints = false;
	self.imageRequestPending = FALSE;
    }
    return self;
}

- (instancetype)initWithImage:(UIImage *)image
{
    if (self = [super initWithImage:image]) {
        self.prevWidth = 0;
        self.prevHeight = 0;
        self.translatesAutoresizingMaskIntoConstraints = false;
	self.imageRequestPending = FALSE;
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        self.prevWidth = 0;
        self.prevHeight = 0;
        self.translatesAutoresizingMaskIntoConstraints = false;
	self.imageRequestPending = FALSE;
    }
    return self;
}

// lazy initialization
- (NSMutableArray *)images
{
    if (!_images) {
        _images = [[NSMutableArray alloc] init];
    }
    return _images;
}

- (FWImage *)getImageForWidth:(NSInteger)width
{
    for (int i = 0; i < self.images.count; i++) {
        FWImage *image = (FWImage *)self.images[i];
        if (image.width >= width) {
            return image;
        }
    }
    if (self.images.count != 0) {
        return (FWImage *)[self.images lastObject];
    } else {
        return nil;
    }
}

- (void)layoutSubviews {
    [super layoutSubviews];
    NSInteger width = (NSInteger)self.frame.size.width;
    NSInteger height = (NSInteger)self.frame.size.height;
    if (width != self.prevWidth || height != self.prevHeight) {
        [self cancelImageRequest];

        self.prevWidth = width;
        self.prevHeight = height;
	
	if (width > 0 && height > 0) {
	    self.imageRequestPending = YES;

            FWImage * bestImage = [self getImageForWidth:width];
            NSString * bestUrl = nil;
            if (bestImage != nil) bestUrl = bestImage.url;

            if ([self.delegate respondsToSelector:@selector(fwImageView:didChangeSize:ofImageUrl:)]) {
                [self.delegate fwImageView:self didChangeSize:self.frame.size ofImageUrl:bestUrl];
            }
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
}

- (void)clear
{
    [self cancelImageRequest];
    self.image = nil;
    [self.images removeAllObjects];
}

- (void)updateContentMode {
    if (self.image != nil && self.prevWidth > 0 && self.prevHeight > 0 &&
	(self.contentMode == UIViewContentModeCenter || self.contentMode == UIViewContentModeScaleAspectFit)) {
        if (self.image.size.width <= self.prevWidth && self.image.size.height <= self.prevHeight) {
            self.contentMode = UIViewContentModeCenter;
        } else {
            self.contentMode = UIViewContentModeScaleAspectFit;
        }
    }
}

- (void)updateVisibility:(CGRect)bounds
{

}

- (void)cancelImageRequest {
#if 0
    if (self.imageRequestPending && [self.delegate respondsToSelector:@selector(didCancelImageRequest:)]) {
        self.imageRequestPending = NO;
        [self.delegate didCancelImageRequest:self];
    }
#endif
}

- (void)dealloc {
    [self cancelImageRequest];
}

@end
