#include "iOSMainThread.h"

#include <ValueEvent.h>
#include <TimerEvent.h>

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
        
      case Command::SET_STYLE: {
        NSString * key = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        NSString * value = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        [viewController setStyle:command.getInternalId() key:key value:value];
      }
        break;
            
      case Command::SET_VISIBILITY: {
          [viewController setVisibility:command.getInternalId() visibility:command.getValue()];
      }
        break;
        
      case Command::CREATE_DIALOG: {

      }
      	break;
      	
      case Command::CREATE_ALERT_DIALOG: {
      	
      }
        break;

	  case Command::SET_INT_VALUE: {
        [viewController setIntValue:comamnd.getInternalId() value:command.getIntValue()];
	  }
	    break;

	  case Command::SET_TEXT_VALUE: {
        NSString * value = [NSString stringWithUTF8String:command.getTextValue().c_str()];
	    [viewController setTextValue:comamnd.getInternalId() value:value];
	  }
	    break;
	                
      case Command::LAUNCH_BROWSER: {
        NSString * input_url = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        NSURL *url = [NSURL URLWithString:input_url];
        [[UIApplication sharedApplication] openURL:url];
        // no need to release anything
      }

    }
  }
}

void
iOSMainThread::sendEvent(int internal_id, const Event & ev) {

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
  CFRunLoopStop(CFRunLoopGetCurrent());
}

void
iOSMainThread::sendMemoryWarning() {
  SysEvent ev(SysEvent::MEMORY_WARNING);
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
iOSMainThread::sendTimerEvent(int timerId) {
    TimerEvent ev(timerId);
    Element::postEventToElement(getApplication().getInternalId(), ev);
}
        
