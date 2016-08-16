#ifndef _SPRITE_H_
#define _SPRITE_H_

class Sprite {
 public:
  Sprite();
  virtual ~Sprite();

  void draw();
  
 private:
  float x = 0, y = 0;
};

#endif
