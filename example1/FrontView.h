#ifndef _FRONTVIEW_H_
#define _FRONTVIEW_H_

#include <FrameView.h>
#include <LinearLayout.h>
#include <Button.h>

#define ID_SHOW_FRONT_PAGE 1234

class FrontView : public FrameView {
public:
    FrontView(int _id = 0) : FrameView(_id) {
        auto & layout = addChild(std::make_shared<LinearLayout>(FW_VERTICAL));
        layout.addChild(std::make_shared<Button>("Click here!", ID_SHOW_FRONT_PAGE));
    }
private:
    
};

#endif
