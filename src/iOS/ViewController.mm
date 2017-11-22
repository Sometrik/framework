#import "ViewController.h"

#include <FWApplication.h>

#include "iOSMainThread.h"

#include <iostream>
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
  
  cerr << "Starting app\n";
  
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

- (void)createTextField: (int)elementId parent:(int)parentId {
  CGRect someRect = CGRectMake(0.0, 0.0, 100.0, 30.0);
  UITextField* text = [[UITextField alloc] initWithFrame:someRect];
  UIView* view = [[UIView alloc] initWithFrame:someRect];

  // Associate the created view with the specified elementId
  // Add the created view as a child to the object specified by parentId
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
    view.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:view];
    [self.viewsDictionary setObject:view forKey:[NSString stringWithFormat:@"%d", viewId]];
    //UIView *parentView = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", parentId]];
    //[parentView addSubview:view];
}

- (void)createLinearLayoutWithId:(int)viewId parentId:(int)parentId direction:(int)direction
{
    UIStackView *stackView = [[UIStackView alloc] init];
    if (direction == 2) {
        stackView.axis = UILayoutConstraintAxisVertical;
    } else {
        stackView.axis = UILayoutConstraintAxisHorizontal;
    }
    //stackView.si
    stackView.frame = self.view.frame;
    stackView.backgroundColor = UIColor.lightGrayColor;
    [self.viewsDictionary setObject:stackView forKey:[NSString stringWithFormat:@"%d", viewId]];
    UIView *parentView = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", parentId]];
    [parentView addSubview:stackView];
}

- (void)createTextWithId:(int)viewId parentId:(int)parentId
{
    UILabel *label = [[UILabel alloc] init];
    label.frame = CGRectMake(0, 0, 50, 20);
    const char *text = "Hello World";
    label.text = [NSString stringWithUTF8String:text];
    label.backgroundColor = UIColor.blueColor;
    [self.viewsDictionary setObject:label forKey:[NSString stringWithFormat:@"%d", viewId]];
    UIView *parentView = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", parentId]];
    NSLog(@"parentView = %@", parentView.class);
    if ([parentView isKindOfClass:UIStackView.class]) {
        UIStackView *stackView = (UIStackView *)parentView;
        [stackView addArrangedSubview:label];
    } else {
        [parentView addSubview:label];
    }
}

- (void)createButtonWithId:(int)viewId parentId:(int)parentId
{
    UIButton *button = [[UIButton alloc] init];
    [self.viewsDictionary setObject:button forKey:[NSString stringWithFormat:@"%d", viewId]];
    UIView *parentView = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", parentId]];
    [parentView addSubview:button];
}



@end
