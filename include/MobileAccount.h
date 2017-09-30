#ifndef _MOBILEACCOUNT_H_
#define _MOBILEACCOUNT_H_

class MobileAccount {
 public:
  MobileAccount() { }
  MobileAccount(const std::string & _email, const std::string & _language, const std::string & _country)
   : email(_email), language(_language), country(_country) { }
  
  const std::string & getEmail() const { return email; }
  const std::string & getLanguage() const { return language; }
  const std::string & getCountry() const { return country; }

 private:
  std::string email;
  std::string language;
  std::string country;
};

#endif
