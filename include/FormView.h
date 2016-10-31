#ifndef _FORMVIEW_H_
#define _FORMVIEW_H_
#include <FWViewBase.h>

class FormView : public FWViewBase {
 public:
  FormView(FWPlatformBase * platform) : FWViewBase(platform) {
  }

  ~FormView(){

  }

  virtual void setTitle(const char * title);
  virtual const char * getUsername();
  virtual const char * getPassword();
  virtual void submitButtonClicked();


 private:
};

#endif
