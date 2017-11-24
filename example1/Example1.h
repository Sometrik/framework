#include <FWApplication.h>

class TextField;

class Example1 : public FWApplication {
public:
  Example1();
    
    void onCommandEvent(CommandEvent & ev) override;
    
private:
    std::shared_ptr<TextField> textField;
};
