#import "FWPicker.h"

@implementation FWPicker

- (id)init {
    self = [super init];
    if (self) {
        self.options = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)addOption:(NSString*)title {
    NSLog(@"add option %@", title);

    if (title == nil) title = @"";

    if ([self.options count] == 0) {
        [self setTitle:title forState:UIControlStateNormal];
        [self.superview setNeedsLayout];
    }

    if (self.options != nil) {
        [self.options addObject:title];
    }
}

- (void)setSelection:(NSInteger)row {
    if (row >= 0 && row < [self.options count]) {
        [self setTitle:self.options[row] forState:UIControlStateNormal];
    } else {
        [self setTitle:@"" forState:UIControlStateNormal];
    }
}

@end
