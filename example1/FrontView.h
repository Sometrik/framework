#ifndef _FRONTVIEW_H_
#define _FRONTVIEW_H_

#include <FrameView.h>
#include <LinearLayout.h>
#include <Button.h>
#include <TextView.h>

#define ID_SHOW_FRONT_PAGE 1234

class FrontView : public FrameView {
public:
    FrontView(int _id = 0) : FrameView("Front page", _id) {
        style("background", "linear-gradient(#ff0000, #ffff00)");
        auto & layout = addChild(std::make_shared<LinearLayout>(FW_VERTICAL));
	layout.style("margin", 25);
	layout.style("padding", 25);
        auto & button = layout.addChild(std::make_shared<Button>("Click here!", ID_SHOW_FRONT_PAGE));
	button.style("background", "#ffffff");
	button.style("border-radius", 5);
	button.style("margin", 25);
	button.style("color", "#000000");
	button.style("padding", 25);
	auto & textView = layout.addChild(std::make_shared<TextView>("Joku linkki http://www.google.fi/ ja joku toinen http://yle.fi/"));	
	textView.style("height", 100);
	textView.style("border", 1);
    }
private:
    
};

#endif
