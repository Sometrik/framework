#include <FWApplication.h>

class TextField;
class ActionBar;
class NavigationDrawer;

class Example1 : public FWApplication {
public:
  Example1();
    
    void onCommandEvent(CommandEvent & ev) override;
    
private:
    std::shared_ptr<TextField> textField;
    std::shared_ptr<ActionBar> actionBar;
    std::shared_ptr<NavigationDrawer> navigationDrawer;
};
