#include <Element.h>
#include <FWPlatform.h>
#include <TextLabel.h>

using namespace std;

void
Element::initialize(FWPlatform * _platform) {
  platform = _platform;
  id = platform->getNextElementId();
}

void
Element::sendMessage(const Message & message){
  platform->sendMessage(message);
}

Element &
Element::addChild(const std::string & text) {
  return addChild(make_shared<TextLabel>(text));
}
