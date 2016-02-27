#ifndef _VBO_H_
#define _VBO_H_

#include <cstddef>
#include <vector>

#include <glm/glm.hpp>

#if 1
#include <TextureRef.h>
#endif

struct vbo_data_s {
  glm::vec2 tex;
  glm::vec3 normal;
  glm::vec3 position;
};

struct vbo_data2_s {
  glm::vec2 tex;
  glm::vec4 color;
  glm::vec3 normal;
  glm::vec3 position;
};

struct vbo_data3_s {
  glm::vec4 color;
  glm::vec3 normal;
  glm::vec3 position;
};

struct billboard_data_s {
  glm::vec3 center_pos;
  glm::uint32 corner_pos;  
  glm::uint32 tex;
};

struct line_data_s {
  unsigned char r, g, b, a;
  glm::vec3 position;
  float node_age, node_radius;
};

struct arc_data_2d_s {
  unsigned char r, g, b, a;
  glm::vec2 position;
};

struct arc_data_3d_s {
  unsigned char r, g, b, a;
  glm::vec3 position, normal;
};

struct node_vbo_s {
  unsigned char r, g, b, a; // 0
  glm::uint32 normal; // 4
  glm::vec3 position; // 8
  float age, size; // 20
  short texture, flags; // 28
};

class VBO {
 public:
  enum DataType { C4F_N3F_V3F, T2F_N3F_V3F, T2F_C4F_N3F_V3F, NODES, BILLBOARDS, EDGES, ARCS_2D, ARCS_3D };
  enum DrawType { NONE = 0, POINTS, LINES, TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN };

  VBO() { }
  virtual ~VBO();

  bool isDefined() const { return vbo != 0; }
  void upload(DataType type, const void * ptr, size_t size);
  void uploadIndices(const void * ptr, size_t size);
  
  void setDrawType(DrawType type) { default_draw_type = type; }

  void draw(DrawType type = NONE);
  void draw(DrawType type, const std::vector<int> & indices);

  void quad2d(float x1, float y1,
	      float x2, float y2,
	      float x3, float y3,
	      float x4, float y4);
  void quad2d(float x1, float y1, float tx1, float ty1,
	      float x2, float y2, float tx2, float ty2,
	      float x3, float y3, float tx3, float ty3,
	      float x4, float y4, float tx4, float ty4);
  void sphere(float radius, unsigned int u, unsigned int v);
  void ring(float outer_radius, float inner_radus, unsigned int n, float dx = 0, float dy = 0);

  unsigned int getNumIndices() const { return num_indices; }
  unsigned int getNumElements() const { return num_elements; }

#if 1
  const canvas::TextureRef & getTexture() const { return texture; }
  void setTexture(const canvas::TextureRef & _texture) { texture = _texture; } 
  void clearTexture() { texture.clear(); }
  bool hasTexture() const { return texture.get() != 0; }
#endif

 protected:
  void bind();
  void unbind();

  DataType getDataType() const { return data_type; }
  int getStride() const { return stride; }

  unsigned int vao = 0, vbo = 0, indexVbo = 0;
#if 1
  canvas::TextureRef texture;
#endif

 private:
  DrawType default_draw_type = NONE;
  DataType data_type = C4F_N3F_V3F;
  int stride = 0;
  unsigned int num_indices = 0, num_elements = 0;  
};

#endif
