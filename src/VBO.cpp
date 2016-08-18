#include "GL.h"
#include "VBO.h"

#include <cassert>
#include <iostream>

using namespace std;

bool VBO::has_vertex_array_objects = false;

static GLenum getGLDrawType(VBO::DrawType type) {
  switch (type) {
  case VBO::NONE: break;
  case VBO::POINTS: return GL_POINTS;
  case VBO::LINES: return GL_LINES;
  case VBO::TRIANGLES: return GL_TRIANGLES;
  case VBO::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
  case VBO::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
  }
  assert(0);
  return 0;
}

VBO::~VBO() {
  clear();
}

void
VBO::clear() {
  if (vao) {
    glDeleteVertexArrays(1, &vao);
    vao = 0;
  }
  if (vbo) {
    glDeleteBuffers(1, &vbo);
    vbo = 0;
  }
  if (indexVbo) {
    glDeleteBuffers(1, &indexVbo);
    indexVbo = 0;    
  }
  num_indices = num_elements = 0;
  data_uploaded = indices_uploaded = false;
}

void
VBO::setPointers() const {
  int n_arrays = 0;
  switch (getDataType()) {
  case T2F_N3F_V3F:
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, getStride(), (void *)(0 * sizeof(float)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, getStride(), (void *)(5 * sizeof(float)));
    n_arrays = 2;
    break;
  case NODE_BILLBOARDS:
    glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_TRUE, getStride(), (void *)(0)); // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, getStride(), (void *)(4)); // center position
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, getStride(), (void *)(16)); // age
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, getStride(), (void *)(20)); // size
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, getStride(), (void *)(24)); // scaling
    glVertexAttribPointer(5, 2, GL_UNSIGNED_SHORT, GL_FALSE, getStride(), (void *)(28)); // texture / flags
    n_arrays = 6;
    break;
  case BILLBOARDS:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, getStride(), (void *)(0 * sizeof(float)));
    glVertexAttribPointer(1, 2, GL_HALF_FLOAT, GL_FALSE, getStride(), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_HALF_FLOAT, GL_FALSE, getStride(), (void *)(4 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_HALF_FLOAT, GL_FALSE, getStride(), (void *)(5 * sizeof(float)));
    glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_TRUE, getStride(), (void *)(6 * sizeof(float)));
    glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE, getStride(), (void *)(7 * sizeof(float)));
    n_arrays = 6;
    break;
  case EDGES:
    glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_TRUE, getStride(), (void *)(0 * sizeof(float)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, getStride(), (void *)(1 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, getStride(), (void *)(4 * sizeof(float)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, getStride(), (void *)(7 * sizeof(float)));
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, getStride(), (void *)(8 * sizeof(float)));
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, getStride(), (void *)(9 * sizeof(float)));
    n_arrays = 6;
    break;
  case ARCS_2D:
    glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_TRUE, getStride(), (void *)(0 * sizeof(float)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, getStride(), (void *)(1 * sizeof(float)));
    n_arrays = 2;
    break;
  case ARCS_3D:
    glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_TRUE, getStride(), (void *)(0 * sizeof(float)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, getStride(), (void *)(1 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, getStride(), (void *)(4 * sizeof(float)));
    n_arrays = 3;
    break;
  }
  for (int i = 0; i < 6; i++) {
    if (i < n_arrays) {
      glEnableVertexAttribArray(i);
    } else if (!hasVertexArrayObjects()) {
      glDisableVertexAttribArray(i);
    }
  }
}

void
VBO::upload(DataType type, const void * ptr, size_t size) {
  // assert(size > 0);
  
  data_type = type;
  switch (type) {
  case VBO::T2F_N3F_V3F: stride = 8 * sizeof(float); break;
  case VBO::NODE_BILLBOARDS: stride = sizeof(node_billboard_vbo_s); break; // ?
  case VBO::BILLBOARDS: stride = sizeof(billboard_data_s); break;
  case VBO::EDGES: stride = sizeof(line_data_s); break;
  case VBO::ARCS_2D: stride = sizeof(arc_data_2d_s); break;
  case VBO::ARCS_3D: stride = sizeof(arc_data_3d_s); break;
  }
  
  num_elements = size / stride;
  if (hasVertexArrayObjects()) {
    if (!vao) glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
  }
  if (!vbo) glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size, ptr, is_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  setPointers();

  data_uploaded = true;
}

void
VBO::uploadIndices(const void * ptr, size_t size) {
  assert(size > 0);

  if (hasVertexArrayObjects()) {
    if (!vao) glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
  }
  if (!indexVbo) glGenBuffers(1, &indexVbo);
  num_indices = size / 4;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, ptr, is_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  
  indices_uploaded = true;
}

void
VBO::draw(DrawType type, const vector<int> & _indices) {
  assert(0);
  const int & first_index = _indices.front();
  glDrawElements(getGLDrawType(type), (GLsizei)_indices.size(), GL_UNSIGNED_INT, &first_index);
  // glBindVertexArray(0);
}

void
VBO::draw(DrawType type) {
  if (!type) type = default_draw_type;
  assert(type);
  assert(vbo);
  assert(data_uploaded);
  if (!hasVertexArrayObjects()) {
    setPointers();
  }
  if (!num_indices) {
    // cerr << "drawing A: " << num_elements << endl;
    glDrawArrays(getGLDrawType(type), 0, num_elements);
  } else {
    assert(indices_uploaded);
    // cerr << "drawing B: " << num_indices << endl;
    glDrawElements(getGLDrawType(type), num_indices, GL_UNSIGNED_INT, 0);
  }
  // glBindVertexArray(0);  
}

void
VBO::quad2d(float x1, float y1,
	    float x2, float y2,
	    float x3, float y3,
	    float x4, float y4) {
  default_draw_type = TRIANGLE_FAN;
  vbo_data_s * data = new vbo_data_s[4];
  unsigned int * indices = new unsigned int[4];
  data[0] = { glm::vec2(0, 0), glm::vec3(0, 0, 1), glm::vec3(x1, y1, 0) };
  data[1] = { glm::vec2(0, 1), glm::vec3(0, 0, 1), glm::vec3(x2, y2, 0) };
  data[2] = { glm::vec2(1, 1), glm::vec3(0, 0, 1), glm::vec3(x3, y3, 0) };
  data[3] = { glm::vec2(1, 0), glm::vec3(0, 0, 1), glm::vec3(x4, y4, 0) };
  upload(T2F_N3F_V3F, data, 4 * sizeof(vbo_data_s));
  indices[0] = 0;
  indices[1] = 1;
  indices[2] = 2;
  indices[3] = 3;
  uploadIndices(indices, 4 * sizeof(unsigned int));
  delete[] indices;
  delete[] data;
}

void
VBO::quad2d(float x1, float y1, float tx1, float ty1,
	    float x2, float y2, float tx2, float ty2,
	    float x3, float y3, float tx3, float ty3,
	    float x4, float y4, float tx4, float ty4) {
  default_draw_type = TRIANGLE_FAN;
  vbo_data_s * data = new vbo_data_s[4];
  unsigned int * indices = new unsigned int[4];
  data[0] = { glm::vec2(tx1, ty1), glm::vec3(0, 0, 1), glm::vec3(x1, y1, 0) };
  data[1] = { glm::vec2(tx2, ty2), glm::vec3(0, 0, 1), glm::vec3(x2, y2, 0) };
  data[2] = { glm::vec2(tx3, ty3), glm::vec3(0, 0, 1), glm::vec3(x3, y3, 0) };
  data[3] = { glm::vec2(tx4, ty4), glm::vec3(0, 0, 1), glm::vec3(x4, y4, 0) };
  upload(T2F_N3F_V3F, data, 4 * sizeof(vbo_data_s));
  indices[0] = 0;
  indices[1] = 1;
  indices[2] = 2;
  indices[3] = 3;
  uploadIndices(indices, 4 * sizeof(unsigned int));
  delete[] indices;
  delete[] data;
}

void
VBO::sphere(float radius, unsigned int u, unsigned int v) {
  default_draw_type = TRIANGLES;
  
  vbo_data_s * data = new vbo_data_s[u * v];
  unsigned int * indices = new unsigned int[6 * u * v];
  
  unsigned int in = 0, vn = 0;
  for (unsigned int i = 0; i < u; i++) {
    for (unsigned int j = 0; j < v; j++) {
      float tx = (float)i / (u - 1), ty = (float)j / (v - 1);
      float i2 = (float)i / u * 2 * M_PI, j2 = ((float)j / v - 0.5f) * M_PI;
      float x = -radius * cos(j2) * cos(i2);
      float y = -radius * sin(j2);
      float z = radius * cos(j2) * sin(i2);
      
      indices[in++] = i * v + j;
      indices[in++] = i * v + (j + 1) % v;
      indices[in++] = ((i + 1) % u) * v + (j + 1) % v;
      indices[in++] = i * v + j;
      indices[in++] = ((i + 1) % u) * v + (j + 1) % v;
      indices[in++] = ((i + 1) % u) * v + j;
      
      data[vn++] = { glm::vec2(tx, ty), glm::vec3(0, 0, 1), glm::vec3(x, y, z) };
    }
  }
  
  uploadIndices(indices, in * sizeof(unsigned int));
  upload(T2F_N3F_V3F, data, vn * sizeof(vbo_data_s));  

  delete[] indices;
  delete[] data;
}

void
VBO::ring(float outer_radius, float inner_radius, unsigned int n, float dx, float dy) {
  default_draw_type = TRIANGLE_STRIP;
  
  vbo_data_s * data = new vbo_data_s[2 * n];
  unsigned int * indices = new unsigned int[2 * n];
    
  unsigned int vn = 0, in = 0;
  for (unsigned int i = 0; i < n; i++) {
    float a = 2 * M_PI / (n - 1);
    float x1 = outer_radius * cos(a) + dx, y1 = outer_radius * sin(a) + dy;
    float x2 = inner_radius * cos(a) + dx, y2 = inner_radius * sin(a) + dy;

    if (i % 2 == 0) {
      indices[in++] = vn + 0;
      indices[in++] = vn + 1;
    } else {
      indices[in++] = vn + 1;
      indices[in++] = vn + 0;
    }
    
    data[vn++] = { glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(x1, y1, 0.0f) };
    data[vn++] = { glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(x2, y2, 0.0f) };    
  }

  cerr << "vbo ring vn = " << vn << ", in = " << in << endl;
  
  upload(T2F_N3F_V3F, data, vn * sizeof(vbo_data_s));  
  uploadIndices(indices, in * sizeof(unsigned int));
  
  delete[] data;
  delete[] indices;
}
