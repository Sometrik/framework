#import "ViewController.h"

#include <FWApplication.h>

#include "iOSMainThread.h"

#include <memory>

using namespace std;

// Pointer to main thread
std::shared_ptr<iOSMainThread> mainThread;

// Declare C++ function
extern FWApplication * applicationMain();

@interface ViewController () <UIScrollViewDelegate>
@property (nonatomic, strong) NSMutableDictionary *viewsDictionary;
@property (nonatomic, strong) UIView *sideMenuView;
@property (nonatomic, strong) UIView *sideMenuBackgroundOverlayView;
@property (nonatomic, strong) UITabBar *tabBar;
@property (nonatomic, strong) UINavigationBar *navBar;
@end

static const NSTimeInterval sidePanelAnimationDuration = 0.4;

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

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    if (self.navBar) {
        [self.view bringSubviewToFront:self.navBar];
    }
    if (self.sideMenuView) {
        [self.view bringSubviewToFront:self.sideMenuBackgroundOverlayView];
        [self.view bringSubviewToFront:self.sideMenuView];
    }
    CGRect tabBarFrame = CGRectMake(0.0, self.view.frame.size.height-self.tabBar.frame.size.height , self.view.frame.size.width, self.tabBar.frame.size.height);
    self.tabBar.frame = tabBarFrame;
    NSLog(@"self.tabBar.frame = %@", NSStringFromCGRect(self.tabBar.frame));
}
- (void)sideMenuBackgroundOverlayViewTapped:(UITapGestureRecognizer *)gesture
{
    [self hideNavigationView];
}

- (UIStatusBarStyle)preferredStatusBarStyle {
    return UIStatusBarStyleLightContent;
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
    [self addView:text withId:viewId];
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
    UIView *view =  [self viewForId:viewId];
  
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
      int width = (int)[value integerValue];
      [view.widthAnchor constraintEqualToConstant:width].active = true;
    }
  } else if ([key isEqualToString:@"height"]) {
    if ([value isEqualToString:@"match-parent"]) {
      
    } else if ([value isEqualToString:@"wrap-content"]) {
      
    } else {
      int height = (int)[value integerValue];
      [view.heightAnchor constraintEqualToConstant:height].active = true;
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
    UIView *view = [self viewForId:viewId];
    
    if ([view isEqual:self.sideMenuView]) {
      if (visibility == 0) {
            [self hideNavigationView];
        } else {
            [self showNavigationView];
        }
    } else {
        BOOL viewHidden = visibility == 0 ? true : false;
        if (view) {
            view.hidden = viewHidden;
        }
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
    [self addView:view withId:viewId];
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
    stackView.distribution = UIStackViewDistributionFill;
    // stackView.distribution = UIStackViewDistributionFillProportionally;
    stackView.alignment = UIStackViewAlignmentFill;
    stackView.translatesAutoresizingMaskIntoConstraints = false;
    stackView.frame = self.view.frame;
  
    [self addView:stackView withId:viewId];
    [self addToParent:parentId view:stackView];
}

- (void)createFrameLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIView *view = [[UIView alloc] init];
    view.tag = viewId;
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
}

- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString*)value
{
    UILabel *label = [[UILabel alloc] init];
    label.tag = viewId;
    label.text = value;
    [self addView:label withId:viewId];
    [self addToParent:parentId view:label];
}

- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption
{
    UIButton *button = [[UIButton alloc] init];
    button.tag = viewId;
    [button setTitle:caption forState:UIControlStateNormal];
    [button addTarget:self action:@selector(buttonPushed:) forControlEvents:UIControlEventTouchUpInside];
    [self addView:button withId:viewId];
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
    [self addView:sw withId:viewId];
    [self addToParent:parentId view:sw];
}

- (void)switchToggled:(UISwitch *)sender
{
    int viewId = (int)sender.tag;
    NSLog(@"viewId = %d", viewId);
    BOOL switchState = sender.on;
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
    [self addView:imageView withId:viewId];
    [self addToParent:parentId view:imageView];
}

- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIScrollView * scrollView = [[UIScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.contentSize = CGSizeMake(self.view.bounds.size.width, 4000);
    scrollView.frame = self.view.frame;
    scrollView.clipsToBounds = YES;
    scrollView.delegate = self;
    [self addView:scrollView withId:viewId];
    [self addToParent:parentId view:scrollView];
}

- (void)calculateScrollViewContentSize:(int)scrollViewId
{
    UIView *possibleScrollView = [self viewForId:scrollViewId];
    if (possibleScrollView && [possibleScrollView isKindOfClass:[UIScrollView class]]) {
        UIScrollView *scrollView = (UIScrollView *)possibleScrollView;
        CGRect contentRect = CGRectZero;
        for (UIView *view in scrollView.subviews) {
            contentRect = CGRectUnion(contentRect, view.frame);
        }
        scrollView.contentSize = CGSizeMake(scrollView.contentSize.width, contentRect.size.height);
    }
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (!scrollView.pagingEnabled) { // Do nothing if scrollView has paging enabled
        NSLog(@"scrollView scrolled");
    }
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    NSLog(@"scrollView did end decelerating");
}

- (void)createPageLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIScrollView * scrollView = [[UIScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.pagingEnabled = YES;
    scrollView.contentSize = CGSizeMake(0, self.view.bounds.size.height);
    scrollView.frame = self.view.frame;
    scrollView.clipsToBounds = YES;
    scrollView.delegate = self;
    [self addView:scrollView withId:viewId];
    [self addToParent:parentId view:scrollView];
}

- (void)createEventLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIView *view = [[UIView alloc] init];
    view.tag = viewId;
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(viewTapped:)];
    [view addGestureRecognizer:tapGestureRecognizer];
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
}

- (void)createNavigationBar:(int)viewId parentId:(int)parentId
{
    // Create navigation bar with a button for opening side menu
    UINavigationBar *navBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 50)];
    UINavigationItem *navItem = [[UINavigationItem alloc] initWithTitle:@"title"];
    
    UIBarButtonItem *menuButton = [[UIBarButtonItem alloc] initWithTitle:@"Menu" style:UIBarButtonItemStylePlain target:self action:@selector(showNavigationView)];
    navItem.leftBarButtonItem = menuButton;
    [navBar setItems:@[navItem]];
    navBar.translucent = YES;
    self.navBar = navBar;
    [self.view addSubview:navBar];
}

- (void)createTabBar:(int)viewId parentId:(int)parentId
{
    UITabBar *tabBar = [[UITabBar alloc] init];
    tabBar.tag = viewId;
    // tabBar.contentMode = UIViewContentModeBottom;
    // [tabBar setAutoresizingMask:UIViewAutoresizingFlexibleTopMargin];
    
    self.tabBar = tabBar;
    [self addView:tabBar withId:viewId];
    //[tabBar.bottomAnchor constraintEqualToAnchor:parentView.bottomAnchor];
    [self addToParent:parentId view:tabBar];
    self.tabBar.items = [[NSArray alloc] init];
    // Put tabbar to the bottom of the view
}

- (void)createTabBarItem:(int)viewId parentId:(int)parentId title:(NSString *)title
{
    UIView *parentView = [self viewForId:parentId];
    
    if ([parentView isKindOfClass:UITabBar.class]) {
        UITabBar *tabBar = (UITabBar *)parentView;
        
        UITabBarItem * tabBarItem = [[UITabBarItem alloc] initWithTitle:title image:nil tag:viewId];
      
        NSMutableArray *items = tabBar.items.mutableCopy;
        if (items != nil) {
            [items addObject:tabBarItem];
            [tabBar setItems:(NSArray *)items animated:false];
        }
    }
}

- (void)createNavigationView:(int)viewId
{
    CGRect viewFrame = self.view.bounds;
    CGRect frame = CGRectMake(CGRectGetMinX(viewFrame), CGRectGetMinY(viewFrame), CGRectGetWidth(viewFrame)-100, CGRectGetHeight(viewFrame));
    self.sideMenuView = [[UIView alloc] initWithFrame:frame];
    self.sideMenuView.tag = viewId;
    self.sideMenuView.hidden = YES;
    self.sideMenuView.layer.shadowColor = [UIColor blackColor].CGColor;
    self.sideMenuView.layer.shadowOpacity = 1.0;
    self.sideMenuView.layer.shadowRadius = 7.5;
    self.sideMenuView.layer.shadowOffset = CGSizeMake(1, 4);
    // create backgroundoverlay view that's behind sidePanel and if clicked closes the panel
    
    self.sideMenuBackgroundOverlayView = [[UIView alloc] initWithFrame:self.view.bounds];
    self.sideMenuBackgroundOverlayView.hidden = YES;
    self.sideMenuBackgroundOverlayView.backgroundColor = UIColor.blackColor;
    [self.view addSubview:self.sideMenuBackgroundOverlayView];
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(sideMenuBackgroundOverlayViewTapped:)];
    [self.sideMenuBackgroundOverlayView addGestureRecognizer:tapGestureRecognizer];
    [self.view addSubview:self.sideMenuView];
    self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, -CGRectGetWidth(self.sideMenuView.frame), 0.0);
    [self addView:self.sideMenuView withId:viewId];
}

- (void)showNavigationView
{
    self.sideMenuView.hidden = NO;
    self.sideMenuBackgroundOverlayView.hidden = NO;
    self.sideMenuBackgroundOverlayView.alpha = 0.0;
    if (self.sideMenuView) {
        [UIView animateWithDuration:sidePanelAnimationDuration animations:^{
            self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, CGRectGetWidth(self.sideMenuView.frame), 0.0);
            self.sideMenuBackgroundOverlayView.alpha = 0.5;
        }];
    }
}

- (void)hideNavigationView
{
    if (self.sideMenuView) {
        [UIView animateWithDuration:sidePanelAnimationDuration animations:^{
            self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, -(CGRectGetWidth(self.sideMenuView.frame)), 0.0);
            self.sideMenuBackgroundOverlayView.alpha = 0.0;
        } completion:^(BOOL finished) {
            if (finished) {
                self.sideMenuView.hidden = YES;
                self.sideMenuBackgroundOverlayView.hidden = YES;
            }
        }];
    }
}

- (void)createActivityIndicatorWithId:(int)viewId parentId:(int)parentId
{
    UIActivityIndicatorView * view = [[UIActivityIndicatorView alloc] init];
    view.tag = viewId;
    [self addView:view withId:viewId];
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
  
    [self addView:dialog withId:viewId];
    [self.view addSubview:dialog];
}

- (void)createTimer:(int)viewId interval:(double)interval
{
  [NSTimer scheduledTimerWithTimeInterval:interval
                                   target:self
                                 selector:@selector(sendTimerEvent:)
                                 userInfo:nil
                                  repeats:YES];
}

- (void)sendTimerEvent:(NSTimer *)timer
{
    mainThread->sendTimerEvent(1);
}

- (void)viewTapped:(UIView *)sender
{
    int viewId = (int)sender.tag;
    NSLog(@"viewId = %d", viewId);
    [self sendIntValue:viewId value:viewId];
}

- (void)addToParent:(int)parentId view:(UIView*)view
{
    UIView *parentView = [self viewForId:parentId];
  
    BOOL add_basic_constraints = NO;
  
    if ([parentView isKindOfClass:UIStackView.class]) {
        UIStackView *stackView = (UIStackView *)parentView;
        [stackView addArrangedSubview:view];
    } else if ([parentView isKindOfClass:UIScrollView.class]) {
        NSUInteger pos = [[parentView subviews] count];
        [parentView addSubview:view];
        UIScrollView * scrollView = (UIScrollView *)parentView;
        if (scrollView.pagingEnabled) {
            int pageWidth = self.view.bounds.size.width;
            // int pageHeight = self.view.bounds.size.height;
            // view.frame = CGRectMake(pos * pageWidth, 0, pageWidth, pageHeight);
            scrollView.contentSize = CGSizeMake(scrollView.contentSize.width + pageWidth, scrollView.contentSize.height);
          
            NSLayoutConstraint *widthConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeWidth multiplier:1 constant:0];
            //NSLayoutConstraint *heightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeHeight multiplier:1 constant:0];
            NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
            NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:pos * pageWidth];
            [view.superview addConstraints:@[widthConstraint, topConstraint, leftConstraint]];
        } else {
            view.frame = parentView.frame;
            add_basic_constraints = YES;
        }
    } else {
        view.frame = parentView.frame;
        [parentView addSubview:view];
        if (![view isKindOfClass:UITabBar.class] && ![view isKindOfClass:UINavigationBar.class]) {
          add_basic_constraints = YES;
        }
    }

    if (add_basic_constraints) {
        NSLayoutConstraint *widthConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeWidth multiplier:1 constant:0];
        NSLayoutConstraint *heightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeHeight multiplier:1 constant:0];
        NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
        NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
        [view.superview addConstraints:@[widthConstraint, heightConstraint, topConstraint, leftConstraint]];
    }
  
    [parentView bringSubviewToFront:view];
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

- (void)reorderChildWithId:(int)viewId parentId:(int)parentId newPosition:(int)position
{
    UIView *parentView = [self viewForId:parentId];
    UIView *childView = [self viewForId:viewId];
    if (parentView && childView) {
        if ([parentView isKindOfClass:[UIStackView class]]) {
            UIStackView *stackView = (UIStackView *)parentView;
            [childView removeFromSuperview];
            [stackView insertArrangedSubview:childView atIndex:position];
            [stackView setNeedsLayout];
            [stackView layoutIfNeeded];
        } else {
            [parentView insertSubview:childView atIndex:position];
        }
    }
}

- (UIView *)viewForId:(int)viewId
{
    return [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", viewId]];
}

- (void)addView:(UIView *)view withId:(int)viewId
{
    [self.viewsDictionary setObject:view forKey:[NSString stringWithFormat:@"%d", viewId]];
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
