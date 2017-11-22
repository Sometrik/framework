#ifndef _IMAGEBUTTON_H_
#define _IMAGEBUTTON_H_

#include <EventLayout.h>
#include <ImageElement.h>

class ImageButton : public EventLayout {
public:
  ImageButton(const std::string & _filename, int _id = 0) : EventLayout(_id) {
    image = std::make_shared<ImageElement>(_filename, _id);
    addChild(image);
  }
  
  void setImageFile(const std::string & filename) {
    image->setImageFile(filename);
  }
  
private:
  std::shared_ptr<ImageElement> image;
};
#endif
