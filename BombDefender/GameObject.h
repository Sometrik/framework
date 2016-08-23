/*
 * Copyright (C) Sometrik Oy 2015
 *
 */

#include <FWContextBase.h>
#include <FWPlatformBase.h>
#include <Sprite.h>
#include <shader_program.h>
#include <VBO.h>

#include "GameRenderer.h"

virtual class GameObject {
public:
	GameObject();
  bool Init();
  void update();
  void onDraw();
  void setPositionX(float _positionX, float _positionY){
  	positionX = _positionX;
  	positionY = _positionY;
  }
  void setHeight(float _height){height = _height}
  void setWidth(float _width){width = _width}
  float getPositionX(){ return positionX; }
  float getPositionY(){ return positionY; }
  float getHeight(){ return height; }
  float getWidth(){ return width; }
  void setSprite(Sprite _sprite){ sprite = _sprite; }

 private:
  float positionX;
  float positionY;
  float height;
  float width;
  Sprite sprite;
};

