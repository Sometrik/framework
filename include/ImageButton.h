#ifndef _IMAGEBUTTON_H_
#define _IMAGEBUTTON_H_

#include <FrameLayout.h>
#include <ImageElement.h>

class ImageButton : public FrameLayout {
public:
  ImageButton(const std::string & _filename, int _id = 0) : FrameLayout(_id) {
    image = std::make_shared<ImageElement>(_filename, _id);
    addChild(image);
    setClickable(true);
  }
  
  void setImageFile(const std::string & filename) {
    image->setImageFile(filename);
  }
  
private:
  std::shared_ptr<ImageElement> image;
};
#endif
