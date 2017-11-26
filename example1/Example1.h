#include <FWApplication.h>

class TextField;
class ActionBar;
class NavigationDrawer;
class LinearLayout;

class Example1 : public FWApplication {
public:
  Example1();
    
    void onCommandEvent(CommandEvent & ev) override;
    void onTimerEvent(TimerEvent & ev) override;
    
private:
    std::shared_ptr<TextField> textField;
    std::shared_ptr<ActionBar> actionBar;
    std::shared_ptr<NavigationDrawer> navigationDrawer;
    std::shared_ptr<LinearLayout> scrollContent;
};
