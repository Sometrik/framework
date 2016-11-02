#include <Element.h>
#include <FWPlatform.h>

void
Element::initialize(FWPlatform * _platform) {
  platform = _platform;
  id = platform->getNextElementId();
}


void
Element::sendMessage(const Message & message){
  platform->sendMessage(message);
}

