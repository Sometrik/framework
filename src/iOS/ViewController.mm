#import "ViewController.h"

#include "OpenGLView.h"

#include <iostream>

using namespace std;

@interface ViewController ()
@end

@implementation ViewController

- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator
{
  [super viewWillTransitionToSize: size withTransitionCoordinator:coordinator];
  [coordinator animateAlongsideTransition: nil completion: ^(id _Nonnull context) {
    OpenGLView * v = (OpenGLView *)self.view;
    [v drawView2];
  }];
}

@end
