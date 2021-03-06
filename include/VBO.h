#ifndef _VBO_H_
#define _VBO_H_

#include <glm/glm.hpp>

#include <cstddef>
#include <vector>
#include <memory>

namespace canvas {
  class Texture;
};

struct vbo_data_s {
  glm::vec2 tex;
  glm::vec3 normal;
  glm::vec3 position;
};

struct billboard_data_s {
  glm::vec3 center_pos;
  glm::vec2 offset;
  glm::vec2 corner_pos;  
  glm::vec2 tex;
  glm::uint32 color1, color2;
};

struct line_data_s {
  glm::uint32 color;
  glm::vec3 position;
  glm::vec3 other_dir;
  glm::vec2 node_size;
};

struct arc_data_2d_s {
  glm::uint32 color;
  glm::vec2 position;
};

struct arc_data_3d_s {
  glm::uint32 color;
  glm::vec3 position, normal;
};

struct node_billboard2_vbo_s {
  glm::uint32 color; // 0
  glm::vec3 center_pos; // 4
  unsigned short texture, pow; // 20
  
  float node_size_x, node_size_y;
  unsigned char aspect_ratio, offset, dummy1, dummy2;
};

struct node_billboard3_vbo_s {
  glm::uint32 color; // 0
  glm::vec3 center_pos; // 4
  unsigned short texture, pow; // 16
  
  glm::uint32 node_size; // 20
  int node_id;
  float aspect_ratio;
};

class VBO {
 public:
  enum DataType { T2F_N3F_V3F = 1, NODE_BILLBOARDS2, NODE_BILLBOARDS3, BILLBOARDS, EDGES, ARCS_2D, ARCS_3D };
  enum DrawType { NONE = 0, POINTS, LINES, TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN };
  
  VBO(bool _is_dynamic = true) : is_dynamic(_is_dynamic) { }
  VBO(const VBO & other) = delete;
  VBO & operator=(const VBO & other) = delete;
  
  virtual ~VBO();

  bool isDefined() const {
    if (hasVertexArrayObjects()) {
      return vao != 0;
    } else {
      return vbo != 0;
    }
  }
  unsigned int getVertexArrayId() const { return vao; }
  unsigned int getVertexBufferId() const { return vbo; }
  unsigned int getIndexBufferId() const { return indexVbo; }

  void upload(DataType type, const void * ptr, size_t size);
  void uploadIndexArray(const unsigned short * ptr, size_t n);
  void clear();
  
  void setDrawType(DrawType type) { default_draw_type = type; }

  void draw(DrawType type = NONE);

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
  unsigned int getNumVertices() const { return num_vertices; }

  void setBaseInstanceSize(unsigned int n) { base_instance_size = n; }

  const std::shared_ptr<canvas::Texture> & getTexture() const { return texture; }
  void setTexture(const std::shared_ptr<canvas::Texture> & _texture) { texture = _texture; } 
  void clearTexture() { texture.reset(); }
  bool hasTexture() const { return texture.get() != 0; }

  void setPointers() const;
    
  static bool hasVertexArrayObjects() { return has_vertex_array_objects; }
  static void setHasVertexArrayObjects(bool t) { has_vertex_array_objects = t; }

  static bool hasInstancing() { return has_instancing; }
  static void setHasInstancing(bool t) { has_instancing = t; }

 protected:
  DataType getDataType() const { return data_type; }
  int getStride() const { return stride; }

  unsigned int vao = 0, vbo = 0, indexVbo = 0;
  std::shared_ptr<canvas::Texture> texture;

 private:
  DrawType default_draw_type = NONE;
  DataType data_type = T2F_N3F_V3F;
  int stride = 0;
  unsigned int num_indices = 0, num_vertices = 0, base_instance_size = 0;
  bool is_dynamic;
  bool data_uploaded = false, indices_uploaded = false;

  static bool has_vertex_array_objects, has_instancing;
};

#endif
