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

  float getX() const { return x; }
  float getY() const { return y; }
  float getWidth() const { return width; }
  float getHeight() const { return height; }

  void setX(float _x) { x = _x; }
  void setY(float _y) { y = _y; }
  void setWidth(float _width) { width = _width; }
  void setHeight(float _height) { height = _height; }
  
 private:
  float x = 0, y = 0, width = 0, height = 0;
  canvas::TextureRef texture;
};

#endif
