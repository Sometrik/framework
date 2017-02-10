#ifndef _MOBILEACCOUNT_H_
#define _MOBILEACCOUNT_H_

class MobileAccount {
 public:
  MobileAccount(const char * _email, const char * _language, const char * _country = "") : email(_email), language(_language), country(_country) { }
  
  const char * getEmail() { return email; }
  const char * getLanguage() { return language; }
  const char * getCountry() { return country; }

 private:
 const char * email;
 const char * language;
 const char * country;

};

#endif
