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
  mainThread->sendMemoryWarning();
  [super didReceiveMemoryWarning];
}

- (void)createTextFieldWithId: (int)viewId parentId:(int)parentId {
    CGRect someRect = CGRectMake(0.0, 0.0, 100.0, 30.0);
    UITextField* text = [[UITextField alloc] initWithFrame:someRect];
    text.tag = viewId;
    text.borderStyle = UITextBorderStyleRoundedRect;
    text.font = [UIFont systemFontOfSize:15];
    text.placeholder = @"enter text";
    text.autocorrectionType = UITextAutocorrectionTypeNo;
    text.keyboardType = UIKeyboardTypeDefault;
    text.returnKeyType = UIReturnKeyDone;
    text.clearButtonMode = UITextFieldViewModeWhileEditing;
    text.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    [text addTarget:self action:@selector(textFieldChanged:) forControlEvents:UIControlEventEditingChanged];
    [self.viewsDictionary setObject:text forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:text];
  }

- (void)textFieldChanged:(UITextField *)sender
{
    int viewId = (int)sender.tag;
    NSLog(@"viewId = %d", viewId);
    NSLog(@"textField.text = %@", sender.text);
    [self sendTextValue:viewId value:sender.text];
}

- (void)setStyle: (int)viewId key:(NSString *)key value:(NSString *)value {
  UIView *view = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", viewId]];
  
  if ([key isEqualToString:@"background-color"]) {
    view.backgroundColor = [self colorFromHexString:value];
  }
  
  if ([view isKindOfClass:UILabel.class]) {
    UILabel *label = (UILabel *)view;
    
    if ([key isEqualToString:@"font-size"]) {
      int b = (int)[value integerValue];
      label.font = [label.font fontWithSize:b];
    }
  }
}

- (void)setVisibility:(int)viewId visibility:(int)visibility
{
    UIView *view = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", viewId]];
    bool viewHidden = visibility == 0 ? true : false;
    if (view) {
        view.hidden = viewHidden;
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
    view.tag = viewId;
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
    [self.viewsDictionary setObject:stackView forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:stackView];
}

- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString*)value
{
    UILabel *label = [[UILabel alloc] init];
    label.tag = viewId;
    //label.frame = CGRectMake(0, 0, 50, 20);
    label.text = value;
    [self.viewsDictionary setObject:label forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:label];
}

- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption
{
    UIButton *button = [[UIButton alloc] init];
    button.tag = viewId;
    [button setTitle:caption forState:UIControlStateNormal];
    [button addTarget:self action:@selector(buttonPushed:) forControlEvents:UIControlEventTouchUpInside];
    [self.viewsDictionary setObject:button forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:button];
}

- (void)buttonPushed:(UIButton *)sender
{
    int viewId = (int)sender.tag;
    NSLog(@"viewId = %d", viewId);
    NSLog(@"buttonTitle = %@", sender.titleLabel.text);
    [self sendIntValue:viewId value:viewId];
}

- (void)createSwitchWithId:(int)viewId parentId:(int)parentId
{
    UISwitch *sw = [[UISwitch alloc] init];
    sw.tag = viewId;
    [sw addTarget:self action:@selector(switchToggled:) forControlEvents:UIControlEventValueChanged];
    [self.viewsDictionary setObject:sw forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:sw];
}

- (void)switchToggled:(UISwitch *)sender
{
    int viewId = (int)sender.tag;
    NSLog(@"viewId = %d", viewId);
    bool switchState = sender.on;
    int value = switchState ? 1 : 0;
    [self sendIntValue:viewId value:value];
    NSLog(@"Switch is %@", switchState ? @"on" : @"off");
}

- (void)createImageWithId:(int)viewId parentId:(int)parentId filename:(NSString *)filename
{
    UIImage *image = [UIImage imageNamed:filename];
    UIImageView *imageView = [[UIImageView alloc] initWithImage:image];
    imageView.tag = viewId;
    imageView.contentScaleFactor = 1.0f;
    imageView.contentMode = UIViewContentModeScaleAspectFit;
    [self addToParent:parentId view:imageView];
}

- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId
{
  
}

- (void)createEventLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIView *view = [[UIView alloc] init];
    view.tag = viewId;
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(viewTapped:)];
    [view addGestureRecognizer:tapGestureRecognizer];
    [self.viewsDictionary setObject:view forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:view];
}

- (void)viewTapped:(UIView *)sender
{
    int viewId = (int)sender.tag;
    NSLog(@"viewId = %d", viewId);
    [self sendIntValue:viewId value:viewId];
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

- (void)setIntValue:(int)viewId value:(int)value
{
	// if view is switch, set state to value
}

- (void)setTextValue:(int)viewId value:(NSString *)value;
{
	// if view is text field or label, set the text
}

- (void)setImage:(int)viewId data:(UIImage *)data
{
	// if view is image, set the content
}

- (void)addImageUrl:(int)viewId url:(NSString *)url width:(int)width height:(int)height
{
  
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

- (UIColor *)colorFromHexString:(NSString *)hexString {
  unsigned rgbValue = 0;
  NSScanner *scanner = [NSScanner scannerWithString:hexString];
  [scanner setScanLocation:1]; // bypass '#' character
  [scanner scanHexInt:&rgbValue];
  return [UIColor colorWithRed:((rgbValue & 0xFF0000) >> 16)/255.0 green:((rgbValue & 0xFF00) >> 8)/255.0 blue:(rgbValue & 0xFF)/255.0 alpha:1.0];
}

@end
