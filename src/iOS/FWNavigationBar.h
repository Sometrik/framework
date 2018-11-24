#import <UIKit/UIKit.h>

@interface FWNavigationBar : UINavigationBar
- (instancetype)init;
- (void)setTitles:(NSString*)title subtitle:(NSString*)subtitle;

@property (nonatomic, strong) UILabel *navBarTitle;
@property (nonatomic, strong) UILabel *navBarSubtitle;
@property (nonatomic, strong) UINavigationItem *navItem;
@end
