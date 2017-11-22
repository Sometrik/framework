#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator;
- (void)didReceiveMemoryWarning;

- (void)createTextField: (int)elementId parent:(int)parentId;
- (void)setStyle: (int)elementId key:(NSString *)key value:(NSString *)value;
@end
