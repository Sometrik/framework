#ifndef _IMAGEDIALOG_H_
#define _IMAGEDIALOG_H_

#include <Dialog.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <Button.h>
#include <ImageElement.h>
#include <TextField.h>
#include <Picker.h>
#include <Logger.h>

#include <memory>

class ImageDialog : public Dialog {
public:
  ImageDialog() : Dialog("Image") {
    auto layout = std::make_shared<LinearLayout>(FW_VERTICAL);
    layout->style("width", "match-parent");
    addChild(layout);
    
    auto label = std::make_shared<TextLabel>("Hello!");
    layout->addChild(label);
    
    auto image = std::make_shared<ImageElement>();
    image->style("width", "match-parent");
    image->style("height", 256);
    image->style("border", "#ffffff");
    layout->addChild(image);

    auto textField = std::make_shared<TextField>();
    textField->style("width", "match-parent");
    layout->addChild(textField);

    auto picker = std::make_shared<Picker>();
    picker->addOption(0, "Option 1");
    picker->addOption(2, "Option 2");
    picker->addOption(3, "Option 3");
    picker->style("width", "match-parent");
    picker->style("color", "#000000");
    layout->addChild(picker);
    
    auto button = std::make_shared<Button>("OK", 123);
    layout->addChild(button);
  }
  
  void onCommandEvent(CommandEvent & ev) override {
    if (ev.getElementId() == 123) {
      getApplication().getLogger().println("Ending dialog");
      endModal(1);
    }
  }
};

#endif
