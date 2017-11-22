#include "iOSMainThread.h"

void
iOSMainThread::sendCommands(const std::vector<Command> & commands) {
  for (auto & command : commands) {
    switch (command.getType()) {
      case Command::CREATE_APPLICATION: {

      }
        break;
        
      case Command::CREATE_FRAMEVIEW: {
        
      }
        break;
        
      case Command::CREATE_LINEAR_LAYOUT: {
        
      }
        break;
        
      case Command::CREATE_TEXT: {
        
      }
        break;

      case Command::CREATE_BUTTON: {
	// Create button here with the id command.getChildInternalId() and put it as a child of element identified by command.getInternalId()

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
