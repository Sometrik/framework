#ifndef _PRIMITIVERENDERER_H_
#define _PRIMITIVERENDERER_H_

#include <VBO.h>
#include <shader_program.h>

#include <glm/glm.hpp>

#define COLOR_BUFFER_BIT		1
#define DEPTH_BUFFER_BIT		2
#define STENCIL_BUFFER_BIT		4
#define DEPTH_STENCIL_BUFFER_BIT	8

namespace canvas {
  class Texture;
};
class FWPlatform;
class Sprite;

class PrimitiveRenderer {
 public:
  enum CompositionMode {
    COPY = 0,
    NORMAL,
    MULTIPLY
  };
  
  PrimitiveRenderer(float _display_scale) : display_scale(_display_scale) { };
  virtual ~PrimitiveRenderer() { }

  virtual void renderTexturedWindow(VBO & vbo, const canvas::Texture & texture, const glm::mat4 & mat, float alpha = 1.0f) { }
  virtual void drawSprite(const Sprite & sprite, const glm::mat4 & projMat, const glm::mat4 & mvMat) { }
  virtual void initialize(FWPlatform & platform) { }

  const glm::ivec2 & getDisplaySize() const { return current_display_size; }

  void clear(int clear_bits);
  void invalidateFramebuffer(int bits);
  void setClearColor(float r, float g, float b);
  void colorMask(bool r, bool g, bool b, bool a);
  void viewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
  
  unsigned int getMaxTextureSize() const { return max_texture_size; } 
  bool hasETC1() const { return has_etc1; }
  bool hasDXT1() const { return has_dxt1; }
  bool hasRGTC() const { return has_rgtc; }
  bool hasRGB565() const { return has_rgb565; }

  float getDisplayScale() const { return display_scale; }
  void setDisplayScale(float f) { display_scale = f; }

  void setScreenSize(unsigned int _width, unsigned int _height, unsigned int _actual_width, unsigned int _actual_height) {
    screen_width = _width;
    screen_height = _height;
    actual_screen_width = _actual_width;
    actual_screen_height = _actual_height;
  }

  unsigned int getScreenWidth() const { return screen_width; }
  unsigned int getScreenHeight() const { return screen_height; }
  unsigned int getActualScreenWidth() const { return actual_screen_width; }
  unsigned int getActualScreenHeight() const { return actual_screen_height; }

 protected:
  void initializeBase();

  void pushGroupMarker(const char * name);
  void popGroupMarker();
  
  void blend(bool t);
  void stencilTest(bool t);
  void stencilMask(int m);
  void depthTest(bool t);
  void depthMask(bool m);
  void cullFace(bool t);
  void setLineWidth(float w);
  void setCompositionMode(CompositionMode mode);
  void bind(const canvas::Texture & texture);
  void bind(const VBO & vbo);
  void use(const gpufw::shader_program & program);
  const char * getGLSLVersion() const;
  const char * getModernGLSLVersion() const;

 private:
  unsigned int max_texture_size = 0;
  bool has_etc1 = false, has_dxt1 = false, has_rgtc = false, has_rgb565 = false;

  bool blend_enabled = false;
  bool depth_test_enabled = false;
  bool current_depth_mask = true;
  bool stencil_test_enabled = false;
  int current_stencil_mask = 0xff;
  bool cull_face_enabled = false;
  float current_line_width = 1.0f;
  bool current_red_mask = true, current_green_mask = true, current_blue_mask = true, current_alpha_mask = true;
  CompositionMode current_composition_mode = COPY;
  glm::ivec2 current_display_size;
  float display_scale = 1.0f;
  int current_texture_2d = 0, current_vertex_array = 0, current_program = 0;
  bool is_es3 = false;
  unsigned int screen_width = 0, screen_height = 0, actual_screen_width = 0, actual_screen_height = 0;
  glm::vec3 clear_color;
};

#endif
