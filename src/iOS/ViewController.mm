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

@end
