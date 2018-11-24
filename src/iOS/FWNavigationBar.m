#import "FWNavigationBar.h"

@implementation FWNavigationBar

- (instancetype)init
{
    if (self = [super init]) {
        self.translatesAutoresizingMaskIntoConstraints = false;        
	self.barTintColor = UIColor.whiteColor;
    }
    return self;
}

- (void)setTitles:(NSString*)title subtitle:(NSString*)subtitle
{
    self.navBarTitle.text = title;
    self.navBarSubtitle.text = subtitle;
}

@end
