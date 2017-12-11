#include "iOSMainThread.h"

#include <ValueEvent.h>
#include <TimerEvent.h>
#include <ImageRequestEvent.h>

#import "EventWrapper.h"

using namespace std;

iOSMainThread::iOSMainThread(std::shared_ptr<FWApplication> _application, std::shared_ptr<Runnable> _runnable) : PlatformThread(0, _application, _runnable) {
  defaults = [NSUserDefaults standardUserDefaults];
}

void
iOSMainThread::sendCommands(const std::vector<Command> & commands) {
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
        
      case Command::CREATE_PROGRESSBAR: {
        [viewController createActivityIndicatorWithId:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;

      case Command::CREATE_PAGE_CONTROL: {
	[viewController createPageControlWithId:command.getChildInternalId() parentId:command.getInternalId() numPages:command.getValue()];
      }
	break;

      case Command::CREATE_DIALOG: {
        [viewController createDialogWithId:command.getChildInternalId() parentId:command.getInternalId() ];
      }
        break;

      case Command::SET_STYLE: {
	ViewManager * viewManager = [viewController getViewManager:command.getInternalId()];
	if (viewManager != nil) {
	  NSString * key = [NSString stringWithUTF8String:command.getTextValue().c_str()];
          NSString * value = [NSString stringWithUTF8String:command.getTextValue2().c_str()];
          [viewManager setStyle:key value:value];
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
	  app.addToHistory(command.getChildInternalId());
	  app.setActiveViewId(command.getChildInternalId());

	  NSString * title = [NSString stringWithUTF8String:command.getTextValue().c_str()];
	  [viewController setTitle:title];
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
        NSURL *url = [NSURL URLWithString:input_url];
        [[UIApplication sharedApplication] openURL:url];
        // no need to release anything
      }
        break;
        
      case Command::CREATE_TIMER: {
        [viewController createTimer:command.getInternalId() interval:command.getValue() / 1000.0];
      }
        break;
        
      case Command::UPDATE_PREFERENCE: {
	NSString *storedVal = [NSString stringWithUTF8String:command.getTextValue().c_str()];
	NSString *storedKey = [NSString stringWithUTF8String:command.getTextValue2().c_str()];
	[defaults setObject:storedVal forKey:storedKey];
	[defaults synchronize];
      }
        break;
        
      case Command::DELETE_PREFERENCE: {
        
      }
        break;
        
      case Command::COMMIT_PREFERENCES: {
        
      }
        break;
    }
  }
}

void
iOSMainThread::sleep(double t) {
  usleep((unsigned int)(t * 1000000));
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
iOSMainThread::setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) {

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

void
iOSMainThread::sendIntValue(int viewId, int value) {
    ValueEvent ev(value);
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
  
    ImageRequestEvent ev(ImageRequestEvent::REQUEST, viewId, width, height);
    if (internalFormat != 0) {
      ev.setInternalFormat((canvas::InternalFormat)internalFormat);
    }
    sendEvent(getApplication().getInternalId(), ev);
}
