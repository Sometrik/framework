#include <Element.h>
#include <FWPlatformBase.h>

void
Element::initialize(FWPlatformBase * _platform) {
  platform = _platform;
  id = platform->getNextElementId();
}


void Element::sendMessage(const Message & message){
  platform->sendMessage(message);
}

