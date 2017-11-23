#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator;
- (void)didReceiveMemoryWarning;

- (void)createTextFieldWithId: (int)viewId parentId:(int)parentId;
- (void)setStyle: (int)viewId key:(NSString *)key value:(NSString *)value;
- (void)setVisibility:(int)viewId visibility:(int)visibility;

- (void)createFrameViewWithId:(int)viewId parentId:(int)parentId;
- (void)createLinearLayoutWithId:(int)viewId parentId:(int)parentId direction:(int)direction;
- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString *)value;
- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption;
- (void)createSwitchWithId:(int)viewId parentId:(int)parentId;
- (void)createImageWithId:(int)viewId parentId:(int)parentId filename:(NSString *)filename;
- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId;
- (void)createEventLayoutWithId:(int)viewId parentId:(int)parentId;

- (void)setIntValue:(int)viewId value:(int)value;
- (void)setTextValue:(int)viewId value:(NSString *)value;
- (void)setImage:(int)viewId data:(UIImage *)data;

- (void)addToParent:(int)parentId view:(UIView*)view;

- (void)sendIntValue:(int)viewId value:(int)value;
- (void)sendTextValue:(int)viewId value:(NSString *)value;

@end
