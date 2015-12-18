#include "FWPlatformBase.h"
#include "FWContextBase.h"

bool
FWContextBase::createWindow(const char * title, int requested_width, int requested_height, unsigned int flags) {
  return getPlatform().createContext( this, title, requested_width, requested_height, flags );    
}
