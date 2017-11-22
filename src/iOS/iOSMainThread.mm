#include "iOSMainThread.h"

#include <ValueEvent.h>

void
iOSMainThread::sendCommands(const std::vector<Command> & commands) {
  for (auto & command : commands) {
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
        
      case Command::CREATE_SCROLL_LAYOUT: {
        
      }
        break;
        
      case Command::CREATE_SWITCH: {
        [viewController createSwitch:command.getChildInternalId() parentId:command.getInternalId()];
      }
        break;
        
      case Command::SET_STYLE: {
        NSString * key = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        NSString * value = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        [viewController setStyle:command.getInternalId() key:key value:value];
      }
        break;
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

void
iOSMainThread::sendIntValue(int viewId, int value) {
    ValueEvent ev(value);
    sendEvent(viewId, ev);
}

void
iOSMainThread::sendTextValue(int viewId, const std::string & value) {
    ValueEvent ev(value);
    sendEvent(viewId, ev);
}

class iOSLogger : public Logger {
public:
  iOSLogger(const std::string & _name) : Logger(_name) { }
    
  void println(const char * s) override {
    std::cerr << s << std::endl;
  }
};
        
std::unique_ptr<Logger>
iOSMainThread::createLogger(const std::string & name) const {
  return std::unique_ptr<Logger>(new iOSLogger(name));
};
