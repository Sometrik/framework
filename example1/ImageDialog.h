#ifndef _IMAGEDIALOG_H_
#define _IMAGEDIALOG_H_

#include <Dialog.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <Button.h>
#include <ImageElement.h>

#include <memory>

class ImageDialog : public Dialog {
public:
  ImageDialog() : Dialog("Image") {
    auto layout = std::make_shared<LinearLayout>(FW_VERTICAL);
    addChild(layout);
    
    auto label = std::make_shared<TextLabel>("Hello!");
    layout->addChild(label);
    
    auto image = std::make_shared<ImageElement>();
    image->style("width", 128);
    image->style("height", 256);
    image->style("border", "#ffffff");
    layout->addChild(image);
    
    auto button = std::make_shared<Button>("OK");
    layout->addChild(button);
  }
  
  void onCommandEvent(CommandEvent & ev) override {
    endModal(1);
  }
};

#endif
