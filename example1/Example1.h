#ifndef _EXAMPLE1_H_
#define _EXAMPLE1_H_

#include <FWApplication.h>

class TextField;
class ActionBar;
class NavigationDrawer;
class LinearLayout;
class FrontView;
class FrameView;

class Example1 : public FWApplication {
public:
  Example1();
    
    void onCommandEvent(CommandEvent & ev) override;
    void onTimerEvent(TimerEvent & ev) override;
    void handleImageRequestEvent(ImageRequestEvent & ev) override;
    
private:
    std::shared_ptr<TextField> textField;
    std::shared_ptr<ActionBar> actionBar;
    std::shared_ptr<NavigationDrawer> navigationDrawer;
    std::shared_ptr<LinearLayout> scrollContent;
    std::shared_ptr<FrontView> frontView;
    std::shared_ptr<FrameView> mainView;
};

#endif
