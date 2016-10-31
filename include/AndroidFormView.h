#include <FormView.h>

class AndroidFormView : public FormView {
 public:
  AndroidFormView(FWPlatformBase * platform) : FormView(platform){
//    getPlatform();
  }

  ~AndroidFormView(){

  }

   void setTitle(const char * title){

   }
   const char * getUsername(){
     return "";
   }
   const char * getPassword(){
     return "";
   }
   void submitButtonClicked(){

   }
   void onDraw(){

   }

   void showView(){

   }


 private:
};

