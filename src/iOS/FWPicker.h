#import <UIKit/UIKit.h>

@interface FWPicker : UIButton
- (void)addOption:(NSString*)title;
- (void)setSelection:(NSInteger)row;

@property (nonatomic, strong) NSMutableArray * options;

@end
