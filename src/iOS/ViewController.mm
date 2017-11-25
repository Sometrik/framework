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
    UITextField* text = [[UITextField alloc] init];
    text.tag = viewId;
    text.borderStyle = UITextBorderStyleRoundedRect;
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
    view.backgroundColor = [self colorFromString:value];
  } else if ([key isEqualToString:@"background"]) {
    view.backgroundColor = [self colorFromString:value];
  } else if ([key isEqualToString:@"shadow"]) {
    view.layer.shadowOpacity = 0.25;
    // view.layer.masksToBounds = NO;
    view.layer.shadowRadius = (float)[value floatValue];
    view.layer.shadowOffset = CGSizeMake(0, 0);
  } else if ([key isEqualToString:@"width"]) {
    if ([value isEqualToString:@"match-parent"]) {
      
    } else if ([value isEqualToString:@"wrap-content"]) {
      
    } else {
      CGRect frame = view.frame;
      frame.size.width = (int)[value integerValue];
      [view setFrame:frame];
    }
  } else if ([key isEqualToString:@"height"]) {
    if ([value isEqualToString:@"match-parent"]) {
      
    } else if ([value isEqualToString:@"wrap-content"]) {
      
    } else {
      CGRect frame = view.frame;
      frame.size.height = (int)[value integerValue];
      [view setFrame:frame];
    }
  } else if ([key isEqualToString:@"border-radius"]) {
    view.layer.cornerRadius = (int)[value integerValue];
  } else if ([key isEqualToString:@"border"]) {
    view.layer.borderColor = [self colorFromString:value].CGColor;
    view.layer.borderWidth = 1.0f;
  }
  
  if ([view isKindOfClass:UILabel.class]) {
    UILabel *label = (UILabel *)view;
    
    if ([key isEqualToString:@"font-size"]) {
      int b = (int)[value integerValue];
      label.font = [label.font fontWithSize:b];
    } else if ([key isEqualToString:@"color"]) {
      label.textColor = [self colorFromString:value];
    } else if ([key isEqualToString:@"text-alignment"]) {
      if ([value isEqualToString:@"center"]) {
        label.textAlignment = NSTextAlignmentCenter;
      } else if ([value isEqualToString:@"right"]) {
        label.textAlignment = NSTextAlignmentRight;
      } else {
        label.textAlignment = NSTextAlignmentLeft;
      }
    }
  } else if ([view isKindOfClass:UIButton.class]) {
    UIButton *button = (UIButton *)view;
    
  } else if ([view isKindOfClass:UITextField.class]) {
    UITextField *textField = (UITextField *)view;
    
    if ([value isEqualToString:@"hint"]) {
      textField.placeholder = value;
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
    [self.viewsDictionary setObject:stackView forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:stackView];
}

- (void)createFrameLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIView *view = [[UIView alloc] init];
    view.tag = viewId;
    [self.viewsDictionary setObject:view forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:view];
}

- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString*)value
{
    UILabel *label = [[UILabel alloc] init];
    label.tag = viewId;
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
    [self.viewsDictionary setObject:image forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:imageView];
}

- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIScrollView * scrollView = [[UIScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.contentSize = CGSizeMake(self.view.bounds.size.width, 5 * self.view.bounds.size.height);
    scrollView.frame = self.view.frame;
    scrollView.clipsToBounds = YES;
    [self.viewsDictionary setObject:scrollView forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:scrollView];
}

- (void)createPageLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIScrollView * scrollView = [[UIScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.pagingEnabled = YES;
    scrollView.contentSize = CGSizeMake(0, self.view.bounds.size.height);
    scrollView.frame = self.view.frame;
    scrollView.clipsToBounds = YES;
    [self.viewsDictionary setObject:scrollView forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:scrollView];
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

- (void)createTabBar:(int)viewId parentId:(int)parentId
{
    UITabBar * tabBar = [[UITabBar alloc] init];
    tabBar.tag = viewId;
    tabBar.contentMode = UIViewContentModeBottom;
    [tabBar setAutoresizingMask:UIViewAutoresizingFlexibleTopMargin];
    [self.viewsDictionary setObject:tabBar forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:tabBar];
}

- (void)createTabBarItem:(int)viewId parentId:(int)parentId title:(NSString *)title
{
    UIView *parentView = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", parentId]];
    if ([parentView isKindOfClass:UITabBar.class]) {
        UITabBar *tabBar = (UITabBar *)parentView;
  
        UITabBarItem * tabBarItem = [[UITabBarItem alloc] init];
        tabBarItem.title = title;
        tabBarItem.tag = viewId;
  
        // NSMutableArray * items = (NSMutableArray *)tabBar.items;
        // if (items == nil) {
        //   tabBar.items = items = [[NSMutableArray alloc] init];
        // }
        // [items addObject:tabBarItem];
    }
}

- (void)createActivityIndicatorWithId:(int)viewId parentId:(int)parentId
{
    UIActivityIndicatorView * view = [[UIActivityIndicatorView alloc] init];
    view.tag = viewId;
    [self.viewsDictionary setObject:view forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self addToParent:parentId view:view];
    [view startAnimating];
}

- (void)createDialogWithId:(int)viewId parentId:(int)parentId
{
    UIView * dialog = [[UIView alloc] init];
    dialog.tag = viewId;
    dialog.layer.backgroundColor = [UIColor grayColor].CGColor;
    dialog.layer.borderColor = [UIColor blackColor].CGColor;
    dialog.layer.borderWidth = 1;
    dialog.layer.cornerRadius = 10;
    dialog.layer.shadowColor = [UIColor blackColor].CGColor;
    dialog.layer.shadowOpacity = 1.0;
    dialog.layer.shadowRadius = 7.5;
    dialog.layer.shadowOffset = CGSizeMake(1, 4);
    dialog.clipsToBounds = NO;
  
    CGSize s = self.view.bounds.size;

    dialog.frame = CGRectMake(50, 50, s.width - 100, s.height - 100);
    // dialog.center = CGPointMake(s.width / 2, (s.height / 2) - 65);
  
    [self.viewsDictionary setObject:dialog forKey:[NSString stringWithFormat:@"%d", viewId]];
    [self.view addSubview:dialog];
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
    } else if ([parentView isKindOfClass:UIScrollView.class]) {
        UIScrollView * scrollView = (UIScrollView *)parentView;
        if (scrollView.pagingEnabled) {
            int pos = [[scrollView subviews] count];
            int pageWidth = self.view.bounds.size.width;
            int pageHeight = self.view.bounds.size.height;
            view.frame = CGRectMake(pos * pageWidth, 0, pageWidth, pageHeight);
            scrollView.contentSize = CGSizeMake(scrollView.contentSize.width + pageWidth, scrollView.contentSize.height);
        } else {
            view.frame = parentView.frame;
        }
        [parentView addSubview:view];
    } else {
        view.frame = parentView.frame;
        [parentView addSubview:view];
    }
}

- (void)removeView:(int)viewId
{
    NSString * key = [NSString stringWithFormat:@"%d", viewId];
    UIView *view = [self.viewsDictionary objectForKey:key];
    if (view != nil) {
        [view removeFromSuperview];
        [self.viewsDictionary removeObjectForKey:key];
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

- (UIColor *)colorFromString:(NSString *)hexString {
  unsigned rgbValue = 0;
  NSScanner *scanner = [NSScanner scannerWithString:hexString];
  [scanner setScanLocation:1]; // bypass '#' character
  [scanner scanHexInt:&rgbValue];
  return [UIColor colorWithRed:((rgbValue & 0xFF0000) >> 16)/255.0 green:((rgbValue & 0xFF00) >> 8)/255.0 blue:(rgbValue & 0xFF)/255.0 alpha:1.0];
}

@end
