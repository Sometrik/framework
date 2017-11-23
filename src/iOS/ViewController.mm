#import "ViewController.h"

#include <FWApplication.h>

#include "iOSMainThread.h"

#include <memory>

using namespace std;

// Pointer to main thread
std::shared_ptr<iOSMainThread> mainThread;

// Declare C++ function
extern FWApplication * applicationMain();

@interface ViewController ()
@property (nonatomic, strong) NSMutableDictionary *viewsDictionary;
@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  // Do any additional setup after loading the view, typically from a nib.
  
  // Creating the C++ app
  std::shared_ptr<FWApplication> application(applicationMain());
  
  mainThread = make_shared<iOSMainThread>(application, application);
  mainThread->viewController = self;
//    mainThread->setActualDisplayWidth(screenWidth);
//    mainThread->setActualDisplayHeight(screenHeight);
//    mainThread->setDisplayScale(displayScale);
  
  application->initialize(mainThread.get());
  application->initializeChildren();
}

- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator
{
  [super viewWillTransitionToSize: size withTransitionCoordinator:coordinator];
  [coordinator animateAlongsideTransition: nil completion: ^(id _Nonnull context) {

  }];
}

- (void)didReceiveMemoryWarning {
  
  [super didReceiveMemoryWarning];
}

- (void)createTextFieldWithId: (int)viewId parentId:(int)parentId {
    CGRect someRect = CGRectMake(0.0, 0.0, 100.0, 30.0);
    UITextField* text = [[UITextField alloc] initWithFrame:someRect];
    text.borderStyle = UITextBorderStyleRoundedRect;
    text.font = [UIFont systemFontOfSize:15];
    text.placeholder = @"enter text";
    text.autocorrectionType = UITextAutocorrectionTypeNo;
    text.keyboardType = UIKeyboardTypeDefault;
    text.returnKeyType = UIReturnKeyDone;
    text.clearButtonMode = UITextFieldViewModeWhileEditing;
    text.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    [self.viewsDictionary setObject:text forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:text];
  }

- (void)setStyle: (int)elementId key:(NSString *)key value:(NSString *)value {
  // Find object with the id elementId
  if ([key isEqualToString:@"width"]) {
    // set width of the object to value
  }
}
// Lazy initialization
- (NSMutableDictionary *)viewsDictionary
{
    if (!_viewsDictionary) {
        _viewsDictionary = [[NSMutableDictionary alloc] init];
    }
    return _viewsDictionary;
}

- (void)createFrameViewWithId:(int)viewId parentId:(int)parentId
{
    UIView *view = [[UIView alloc] initWithFrame:self.view.bounds];
    view.backgroundColor = [UIColor darkGrayColor];
    [self.view addSubview:view];
    [self.viewsDictionary setObject:view forKey:[NSString stringWithFormat:@"%d", viewId]];
    //UIView *parentView = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", parentId]];
    //[parentView addSubview:view];
}

- (void)createLinearLayoutWithId:(int)viewId parentId:(int)parentId direction:(int)direction
{
    UIStackView *stackView = [[UIStackView alloc] init];
    if (direction == 1) {
        stackView.axis = UILayoutConstraintAxisVertical;
    } else {
        stackView.axis = UILayoutConstraintAxisHorizontal;
    }
    //stackView.si
    stackView.frame = self.view.frame;
    stackView.backgroundColor = UIColor.lightGrayColor;
    [self.viewsDictionary setObject:stackView forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:stackView];
}

- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString*)value
{
    UILabel *label = [[UILabel alloc] init];
    label.frame = CGRectMake(0, 0, 50, 20);
    label.text = value;
    label.backgroundColor = UIColor.blueColor;
    [self.viewsDictionary setObject:label forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:label];
}

- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption
{
    UIButton *button = [[UIButton alloc] init];
    [button setTitle:caption forState:UIControlStateNormal];
    [self.viewsDictionary setObject:button forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:button];
}

- (void)createSwitchWithId:(int)viewId parentId:(int)parentId
{
    UISwitch *sw = [[UISwitch alloc] init];
    [self.viewsDictionary setObject:sw forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:sw];
}

- (void)createImageWithId:(int)viewId parentId:(int)parentId filename:(NSString *)filename
{
  
}

- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId
{
  
}

- (void)addToParent:(int)parentId view:(UIView*)view
{
    UIView *parentView = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", parentId]];
    if ([parentView isKindOfClass:UIStackView.class]) {
        UIStackView *stackView = (UIStackView *)parentView;
        [stackView addArrangedSubview:view];
    } else {
        [parentView addSubview:view];
    }
}

// This method send changed integer or boolean values back to application.
// For example, a button click sends value 1
- (void)sendIntValue:(int)viewId value:(int)value {
    mainThread->sendIntValue(viewId, value);
}

// This method send changed text values back to application.
- (void)sendTextValue:(int)viewId value:(NSString *)value {
    string s = [value cStringUsingEncoding:NSUTF8StringEncoding];
    mainThread->sendTextValue(viewId, s);
}

@end
