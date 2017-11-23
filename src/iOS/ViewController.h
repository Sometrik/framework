#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator;
- (void)didReceiveMemoryWarning;

- (void)createTextFieldWithId: (int)viewId parentId:(int)parentId;
- (void)setStyle: (int)viewId key:(NSString *)key value:(NSString *)value;

- (void)createFrameViewWithId:(int)viewId parentId:(int)parentId;
- (void)createLinearLayoutWithId:(int)viewId parentId:(int)parentId direction:(int)direction;
- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString *)value;
- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption;
- (void)createSwitchWithId:(int)viewId parentId:(int)parentId;
- (void)createImageWithId:(int)viewId parentId:(int)parentId filename:(NSString *)filename;
- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId;

- (void)addToParent:(int)parentId view:(UIView*)view;

- (void)sendIntValue:(int)viewId value:(int)value;
- (void)sendTextValue:(int)viewId value:(NSString *)value;

@end
