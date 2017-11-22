#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator;
- (void)didReceiveMemoryWarning;

- (void)createTextField: (int)elementId parent:(int)parentId;
- (void)setStyle: (int)elementId key:(NSString *)key value:(NSString *)value;

- (void)createFrameViewWithId:(int)viewId parentId:(int)parentId;
- (void)createLinearLayoutWithId:(int)viewId parentId:(int)parentId direction:(int)direction;
- (void)createTextWithId:(int)viewId parentId:(int)parentId;
- (void)createButtonWithId:(int)viewId parentId:(int)parentId;

@end
