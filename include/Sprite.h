#ifndef _SPRITE_H_
#define _SPRITE_H_

#include<TextureRef.h>

class Sprite {
 public:
  Sprite();
  virtual ~Sprite();

  void draw();
  
  canvas::TextureRef getTexture() const { return texture; }
  void setTexture(canvas::TextureRef _texture) { texture = _texture; }

 private:
  float x = 0, y = 0;
  canvas::TextureRef texture;
};

#endif
