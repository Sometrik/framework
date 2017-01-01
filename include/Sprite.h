#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <memory>

namespace canvas {
  class Texture;
};

#include <glm/glm.hpp>

class Sprite {
 public:
  Sprite();
  virtual ~Sprite();
  
  const std::shared_ptr<canvas::Texture> & getTexture() const { return texture; }
  void setTexture(const std::shared_ptr<canvas::Texture> & _texture) { texture = _texture; }

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
  std::shared_ptr<canvas::Texture> texture;
};

#endif
