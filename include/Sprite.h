#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <TextureRef.h>

#include <glm/glm.hpp>

class Sprite {
 public:
  Sprite();
  virtual ~Sprite();
  
  canvas::TextureRef getTexture() const { return texture; }
  void setTexture(canvas::TextureRef _texture) { texture = _texture; }

  const glm::vec2 & getPosition() const { return position; }
  float getWidth() const { return width; }
  float getHeight() const { return height; }

  void setPosition(const glm::vec2 & _position) { position = _position; }
  void setWidth(float _width) { width = _width; }
  void setHeight(float _height) { height = _height; }

 protected:
  glm::vec2 position;

 private:
  float width = 0, height = 0;
  canvas::TextureRef texture;
};

#endif
