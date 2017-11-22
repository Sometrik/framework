#import "ViewController.h"

#include <FWApplication.h>

#include <iostream>
#include <memory>

using namespace std;

// Pointer to main thread
// std::shared_ptr<iOSMainThread> mainThread;

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
    FWApplication * app = applicationMain();
  
    // mainThread = make_shared<iOSMainThread>(application, application);
    // mainThread->setActualDisplayWidth(screenWidth);
    // mainThread->setActualDisplayHeight(screenHeight);
    // mainThread->setDisplayScale(displayScale);
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

@end
