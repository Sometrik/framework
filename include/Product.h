#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include <string>

class Product {
 public:
  enum Type { INAPP = 1, SUB };
  Product() { }
  
 private:
  std::string productId;
  Type type = INAPP;
  std::string price;
  int price_amount = 0; // micro-units
  std::string price_currency;
  std::string title;
  std::string description;
};

#endif
