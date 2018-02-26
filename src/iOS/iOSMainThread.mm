#include "iOSMainThread.h"

#include <ValueEvent.h>
#include <TimerEvent.h>
#include <ImageRequestEvent.h>
#include <FWViewBase.h>
#include <VisibilityEvent.h>
#include <ScrollChangedEvent.h>

#include "iOSThread.h"

#import "EventWrapper.h"

#include <unordered_set>

using namespace std;

iOSMainThread::iOSMainThread(std::shared_ptr<FWApplication> _application, std::shared_ptr<Runnable> _runnable) : PlatformThread(0, _application, _runnable) {
  defaults = [NSUserDefaults standardUserDefaults];

  FWPreferences preferences;
  NSArray *keys = [[[NSUserDefaults standardUserDefaults] dictionaryRepresentation] allKeys];
  for (NSString * key in keys) {
    NSString * value = [defaults valueForKey:key];
    NSLog(@"loaded data: %@ %@", key, value);  
    if ([value isKindOfClass:[NSString class]]) {
      preferences.setText([key UTF8String], [value UTF8String]);
    }
  }
  _application->setPreferences(preferences);
}

void
iOSMainThread::sendCommands(const std::vector<Command> & commands) {
  unordered_set<int> changedViews;

  for (auto & command : commands) {
    if (command.getType() == Command::CREATE_FRAMEVIEW || command.getType() == Command::CREATE_OPENGL_VIEW) {
      auto & app = getApplication();
      if (!app.getActiveViewId()) {
        app.setActiveViewId(command.getChildInternalId());
      }
    }

    switch (command.getType()) {
      case Command::CREATE_APPLICATION: {
        NSString * appName = [NSString stringWithUTF8String:command.getTextValue().c_str()];
      }
        break;
        
      case Command::CREATE_FRAMEVIEW: {
          [viewController createFrameViewWithId:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::CREATE_LINEAR_LAYOUT: {
          [viewController createLinearLayoutWithId:command.getChildInternalId() parentId:command.getInternalId() direction:command.getValue()];
      }
        break;
        
      case Command::CREATE_EVENT_LAYOUT: {
          [viewController createEventLayoutWithId:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::CREATE_FRAME_LAYOUT: {
          [viewController createFrameLayoutWithId:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::CREATE_PAGER:
      case Command::CREATE_FLIPPER_LAYOUT: {
        [viewController createPageLayoutWithId:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::CREATE_TEXT: {
        NSString * value = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        [viewController createTextWithId:command.getChildInternalId() parentId:command.getInternalId() value:value];
      }
        break;

      case Command::CREATE_BUTTON: {
        NSString * caption = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        [viewController createButtonWithId:command.getChildInternalId() parentId:command.getInternalId() caption:caption];
      }
	break;
        
      case Command::CREATE_TEXTFIELD: {
        [viewController createTextFieldWithId:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::CREATE_IMAGEVIEW: {
        NSString * filename = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        [viewController createImageWithId:command.getChildInternalId() parentId:command.getInternalId() filename:filename];
      }
        break;
        
      case Command::CREATE_SCROLL_LAYOUT: {
        [viewController createScrollLayoutWithId:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::CREATE_SWITCH: {
        [viewController createSwitchWithId:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::CREATE_ACTIONBAR: {
        [viewController createNavigationBar:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::CREATE_NAVIGATIONBAR: {
        [viewController createTabBar:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::CREATE_NAVIGATIONVIEW: {
          [viewController createNavigationView:command.getChildInternalId()];
      }
        break;
            
      case Command::CREATE_NAVIGATIONBAR_ITEM: {
        NSString * title = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        [viewController createTabBarItem:command.getChildInternalId() parentId:command.getInternalId() title:title];
      }
        break;
        
      case Command::CREATE_PROGRESS_SPINNER: {
        [viewController createActivityIndicatorWithId:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;

      case Command::CREATE_PAGE_CONTROL: {
	[viewController createPageControlWithId:command.getChildInternalId() parentId:command.getInternalId() numPages:command.getValue()];
      }
	break;
	
	   case Command::CREATE_PICKER: {
    [viewController createPickerWithId:command.getChildInternalId() parentId:command.getInternalId()];       	
       }
	 break;

      case Command::CREATE_DIALOG: {
        [viewController createDialogWithId:command.getChildInternalId() parentId:command.getInternalId() ];
      }
        break;

      case Command::SET_STYLE: {
	Selector selector = (Selector)command.getValue();
	ViewManager * viewManager = [viewController getViewManager:command.getInternalId()];
	if (viewManager != nil) {
	  NSString * key = [NSString stringWithUTF8String:command.getTextValue().c_str()];
          NSString * value = [NSString stringWithUTF8String:command.getTextValue2().c_str()];
          [viewManager setStyle:key value:value selector:(StyleSelector)selector];
	  changedViews.insert(command.getInternalId());
	}
      }
        break;
            
      case Command::SET_VISIBILITY: {
          [viewController setVisibility:command.getInternalId() visibility:command.getValue()];
      }
        break;
        
      case Command::CREATE_ALERT_DIALOG: {
      	
      }
        break;
        
      case Command::CREATE_ACTION_SHEET: {
        NSString * title = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        [viewController createActionSheetWithId:command.getChildInternalId() parentId:command.getInternalId() title:title];
      }
        break;
            
      case Command::CLEAR: {
          ViewManager * viewManager = [viewController getViewManager:command.getInternalId()];
          if (viewManager) {
              [viewManager clear];
          }
      }
        break;
            
      case Command::REORDER_CHILD: {
          [viewController reorderChildWithId:command.getChildInternalId() parentId:command.getInternalId() newPosition:command.getValue()];
      }
	break;

      case Command::REMOVE_CHILD: {
        
      }
        break;
        
      case Command::DELETE_ELEMENT: {
        [viewController removeView:command.getInternalId()];
      }
        break;
        
      case Command::ADD_IMAGE_URL: {
        ViewManager * viewManager = [viewController getViewManager:command.getInternalId()];
	if (viewManager != nil) {
	  NSString * url = [NSString stringWithUTF8String:command.getTextValue().c_str()];
          [viewManager addImageUrl:url width:command.getWidth() height:command.getHeight()];
	}
      }
        break;
        
      case Command::SET_INT_VALUE: {
	if (command.getInternalId() == 1) {
          auto & app = getApplication();
          
	  if (command.getChildInternalId() != app.getActiveViewId()) {
	     if (app.getActiveViewId()) {
	       [viewController setVisibility:app.getActiveViewId() visibility:0];
	     }

	     app.addToHistory(command.getChildInternalId());
	     app.setActiveViewId(command.getChildInternalId());

	     [viewController setVisibility:app.getActiveViewId() visibility:1];

	     NSString * title = [NSString stringWithUTF8String:command.getTextValue().c_str()];
	     [viewController setTitle:title];
	   }
	} else {
	  ViewManager * viewManager = [viewController getViewManager:command.getInternalId()];
	  if (viewManager != nil) {
	    [viewManager setIntValue:command.getValue()];
	  }
	}
      }
	break;

      case Command::SET_TEXT_VALUE: {
	ViewManager * viewManager = [viewController getViewManager:command.getInternalId()];
	if (viewManager != nil) {
	  NSString * value = [NSString stringWithUTF8String:command.getTextValue().c_str()];
	  [viewManager setTextValue:value];
	}
      }
	break;
	
      case Command::ADD_OPTION: {
        NSString * title = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        [viewController addOption:command.getInternalId() optionId:command.getValue() title:title];
      }
        break;
        
      case Command::LAUNCH_BROWSER: {
        NSString * input_url = [NSString stringWithUTF8String:command.getTextValue().c_str()];
#if 1
	[viewController createWebBrowserWithUrl:(NSString *)input_url];
#else
        NSURL *url = [NSURL URLWithString:input_url];
        [[UIApplication sharedApplication] openURL:url];
        // no need to release anything
#endif
      }
        break;
        
      case Command::CREATE_TIMER: {
        [viewController createTimer:command.getInternalId() interval:command.getValue() / 1000.0];
      }
        break;
        
      case Command::UPDATE_PREFERENCE: {
	NSString *storedKey = [NSString stringWithUTF8String:command.getTextValue().c_str()];
	NSString *storedVal = [NSString stringWithUTF8String:command.getTextValue2().c_str()];
	[defaults setValue:storedVal forKey:storedKey];
      }
        break;
        
      case Command::DELETE_PREFERENCE: {
	NSString *storedKey = [NSString stringWithUTF8String:command.getTextValue().c_str()];
	[defaults setValue:nil forKey:storedKey];        
      }
        break;
        
      case Command::COMMIT_PREFERENCES: {
	[defaults synchronize];        
      }
        break;
    }
  }

  for (auto id : changedViews) {
    ViewManager * viewManager = [viewController getViewManager:id];
    if (viewManager != nil) {
      [viewManager applyStyles:NO];
    }
  }
}

void
iOSMainThread::sleep(double t) {
  usleep((unsigned int)(t * 1000000));
}

std::string
iOSMainThread::loadTextAsset(const char * filename) {
  NSString * filename2 = [NSString stringWithUTF8String:filename];
  NSString * path = [[NSBundle mainBundle] pathForResource:filename2 ofType:nil];
  NSString * content = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:NULL];
  string s = [content cStringUsingEncoding:NSUTF8StringEncoding];
  return s;
}

void
iOSMainThread::sendEvent(int internal_id, const Event & ev) {
    EventWrapper * ew = [[EventWrapper alloc] init];
    ew.targetElementId = internal_id;
    ew.eventPtr = ev.dup();
    [viewController sendEventToMainThread:ew];
}

void
iOSMainThread::handleEventFromThread(int target_element_id, Event * event) {
    Element::postEventToElement(target_element_id, *event);
    delete event;
}

void
iOSMainThread::setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> input) {
  ViewManager * viewManager = [viewController getViewManager:internal_id];
  if (viewManager != nil) {
    int bpp = input->getBytesPerPixel(input->getInternalFormat());
    
    auto cfdata = CFDataCreate(0, input->getData(), bpp * input->getWidth() * input->getHeight());
    auto provider = CGDataProviderCreateWithCFData(cfdata);
    auto colorspace = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrder32Little;
    if (input->getInternalFormat() == canvas::RGBA8) {
      bitmapInfo |= kCGImageAlphaPremultipliedFirst;
    } else {
      bitmapInfo |= kCGImageAlphaNoneSkipFirst;
    }
    auto img = CGImageCreate(input->getWidth(), input->getHeight(), 8, bpp * 8, bpp * input->getWidth(), colorspace, bitmapInfo, provider, 0, true, kCGRenderingIntentDefault);
    
    UIImage * image;
    if ([UIImage respondsToSelector:@selector(imageWithCGImage:scale:orientation:)]) {
      float scale = [[UIScreen mainScreen] scale];
      image = [UIImage imageWithCGImage:img scale:scale orientation:UIImageOrientationUp];
    } else {
      image = [UIImage imageWithCGImage:img];
    }
    
    [viewManager setImage:image];
    
    CGColorSpaceRelease(colorspace);
    CGImageRelease(img);
    CGDataProviderRelease(provider);
    CFRelease(cfdata);
  }
}

void
iOSMainThread::setSurface(int internal_id, canvas::Surface & surface) {

}

int
iOSMainThread::startModal() {
  CFRunLoopRun();
  return getModalResultValue();
}

void
iOSMainThread::endModal(int value) {
  setModalResultValue(value);
  CFRunLoopStop(CFRunLoopGetCurrent());
}

std::shared_ptr<PlatformThread>
iOSMainThread::createThread(std::shared_ptr<Runnable> & runnable) {
  return std::make_shared<iOSThread>(this, application, runnable);
}

void
iOSMainThread::sendMemoryWarning() {
  SysEvent ev(SysEvent::MEMORY_WARNING);
  Element::postEventToElement(getApplication().getInternalId(), ev);
}

void
iOSMainThread::startDebugMode() {
  SysEvent ev(SysEvent::SHOW_DEBUG);
  Element::postEventToElement(getApplication().getInternalId(), ev);
}

bool
iOSMainThread::back() {
  auto & app = getApplication();
  int id = app.popViewBackHistory();
  if (id) {
    if (app.getActiveViewId()) {
      [viewController setVisibility:app.getActiveViewId() visibility:0];
    }
    app.setActiveViewId(id);

    [viewController setVisibility:app.getActiveViewId() visibility:1];

    string title;
    auto e = Element::getRegisteredElement(id);
    if (e) {
	auto e2 = dynamic_cast<FWViewBase*>(e);
	if (e2) title = e2->getTitle().c_str();
    }
    [viewController setTitle:[NSString stringWithUTF8String:title.c_str()]];

    return true;
  } else {
    return false;
  }
}

void
iOSMainThread::sendIntValue(int viewId, int value) {
    ValueEvent ev(value);
    Element::postEventToElement(viewId, ev);
}

void
iOSMainThread::sendVisibilityEvent(int viewId, bool visibility) {
    VisibilityEvent ev(visibility);
    Element::postEventToElement(viewId, ev);
}

void
iOSMainThread::sendScrollChangedEvent(int viewId, int scrollPos, int scrollRem, int height) {
    ScrollChangedEvent ev(scrollPos, scrollRem, height);
    Element::postEventToElement(viewId, ev);
}

void
iOSMainThread::sendTextValue(int viewId, const std::string & value) {
    ValueEvent ev(value);
    Element::postEventToElement(viewId, ev);
}

void
iOSMainThread::sendTimerEvent(int viewId) {
    TimerEvent ev(0);
    Element::postEventToElement(viewId, ev);
}

void
iOSMainThread::sendImageRequest(int viewId, unsigned int width, unsigned int height, const std::string & url, int internalFormat) {
    cerr << "sending image request, width = " << width << ", height = " << height << ", url = " << url << endl;
  
    float scale = [[UIScreen mainScreen] scale];
    if (scale != 1) {
        width = (unsigned int)(width * scale);
        height = (unsigned int)(height * scale);
    }
  
    ImageRequestEvent ev(ImageRequestEvent::REQUEST, viewId, url, width, height);
    if (internalFormat != 0) {
      ev.setInternalFormat((canvas::InternalFormat)internalFormat);
    }
    sendEvent(getApplication().getInternalId(), ev);
}
