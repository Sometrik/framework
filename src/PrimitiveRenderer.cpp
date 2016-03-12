#include "GL.h"
#include "PrimitiveRenderer.h"
#include <VBO.h>

#include "../system/StringUtils.h"

#include <iostream>

using namespace std;

PrimitiveRenderer::PrimitiveRenderer() {
  
}

static set<int> readCompressedFormats() {
  set<int> r;
  GLint num_compressed_format;
  glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num_compressed_format);
  if (num_compressed_format > 0) {
    GLint * compressed_format = new GLint[num_compressed_format * sizeof(GLint)];
    glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, compressed_format);
    for (unsigned int i = 0; i < num_compressed_format; i++) {
      r.insert(compressed_format[i]);
    }
    delete[] compressed_format;
  }
  return r;
}

static set<string> readExtensions() {
  set<string> r;
  GLint numExtensions;
  glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
  if (numExtensions > 0) {
    for (int i = 0; i < numExtensions; i++) {
      const GLubyte * e = glGetStringi(GL_EXTENSIONS, i);
      r.insert((const char *)e);
    }
  }
  return r;
}

void
PrimitiveRenderer::initializeBase() {
#ifdef _WIN32
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    assert(0);
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
  
  glPolygonOffset(1, 2);
  glClearColor(0.98f, 0.98f, 0.98f, 1.0f);
  glActiveTexture(GL_TEXTURE0);

  glStencilFuncSeparate(GL_FRONT, GL_EQUAL, 0, 0xff);
  glStencilFuncSeparate(GL_BACK, GL_NEVER, 0, 0xff);
  glStencilOpSeparate(GL_FRONT, GL_DECR_WRAP, GL_KEEP, GL_KEEP);
  glStencilOpSeparate(GL_BACK, GL_INCR_WRAP, GL_KEEP, GL_KEEP);

#if 0
#if 1
  static const GLfloat light0_pos[4]   = { -50.0f, 50.0f, 0.0f, 0.0f };
#else
  static const GLfloat light0_pos[4]   = { 0.0f, 0.0f, +50.0f, 0.0f };
#endif
  
  // white light
  static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
  static const GLfloat light1_pos[4]   = {  50.0f, 50.0f, 0.0f, 0.0f };
  
  // cold blue light
  static const GLfloat light1_color[4] = { 0.4f, 0.4f, 1.0f, 1.0f };
#endif
  
#if 0
  /* light */
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_color);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_color);
  glEnable(GL_LIGHT1);
#endif

  set<int> compressed_formats = readCompressedFormats();
  set<string> extensions = readExtensions();
  
  // GL_ARB_ES2_compatibility
  // GL_ARB_ES3_compatibility
  // GL_ARB_texture_storage
  // GL_ARB_compressed_texture_pixel_storage
  // GL_EXT_abgr, GL_EXT_bgra

  has_dxt1 = extensions.count("GL_EXT_texture_compression_dxt1");
  has_etc1 = extensions.count("OES_compressed_ETC1_RGB8_texture");
  has_rgtc = extensions.count("GL_ARB_texture_compression_rgtc") || extensions.count("GL_EXT_texture_compression_rgtc");
  
  // GL_IMG_texture_compression_pvrtc"
  // GL_AMD_compressed_ATC_texture / GL_ATI_texture_compression_atitc
  // GL_OES_texture_compression_S3TC / GL_EXT_texture_compression_s3tc  

  // EXT_texture_rg : RED and RG modes

  const char * version_str = (const char *)glGetString(GL_VERSION);
  if (version_str) { 
    // OpenGL ES 3.0 Apple A7 GPU - 75.9.3
    // 3.0 Mesa 11.0.2
    cerr << "got version: " << version_str << endl;
    vector<string> parts = StringUtils::split(version_str);
    if (parts.size() >= 3 && parts[0] == "OpenGL" && parts[1] == "ES") {
      const string & es_version = parts[2];
      cerr << "got OpenGL ES: " << es_version << endl;
      if (es_version == "3.0") {
	cerr << "has etc1" << endl;
	has_etc1 = true;
      }
      has_rgb565 = true;
    } else if (parts.size() >= 2 && parts[1] == "Mesa") {

    } else {
      assert(0);
    }
  }

  int ii;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &ii);
  max_texture_size = ii;
  cerr << "Maximum texture size is " << max_texture_size << endl;

  // MAX_VERTEX_TEXTURE_IMAGE_UNITS => how many textures a vertex shader can read
}

void
PrimitiveRenderer::blend(bool t) {
  if (t != blend_enabled) {
    blend_enabled = t;
    if (t) glEnable(GL_BLEND);
    else glDisable(GL_BLEND);
  }
}

void
PrimitiveRenderer::stencilTest(bool t) {
  if (t != stencil_test_enabled) {
    stencil_test_enabled = t;
    if (t) glEnable(GL_STENCIL_TEST);
    else glDisable(GL_STENCIL_TEST);
  }
}

void
PrimitiveRenderer::stencilMask(int m) {
  if (m != current_stencil_mask) {
    current_stencil_mask = m;
    glStencilMask(m);
  }
}

void
PrimitiveRenderer::depthTest(bool t) {
  if (t != depth_test_enabled) {
    depth_test_enabled = t;
    if (t) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
  }
}

void
PrimitiveRenderer::depthMask(bool m) {
  if (m != current_depth_mask) {
    current_depth_mask = m;
    glDepthMask(m ? GL_TRUE : GL_FALSE);
  }
}

void
PrimitiveRenderer::cullFace(bool t) {
  if (t != cull_face_enabled) {
    cull_face_enabled = t;
    if (t) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);
  }
}

void
PrimitiveRenderer::setLineWidth(float w) {
  if (w != current_line_width) {
    current_line_width = w;
    glLineWidth(w);
  }
}

void
PrimitiveRenderer::bind(const canvas::TextureRef & texture) {
  int id = texture.getTextureId();
  if (!id) {
    cerr << "trying to bind zero tex" << endl;
    assert(0);
  } else if (id != current_texture_2d) {
    current_texture_2d = id;
    glBindTexture(GL_TEXTURE_2D, id);
  }
}

void
PrimitiveRenderer::bind(const VBO & vbo) {
  int a = vbo.getVertexArrayId();
  if (!a) {
    cerr << "trying to bind zero vao" << endl;
    assert(0);
  } else if (a != current_vertex_array) {
    current_vertex_array = a;
    glBindVertexArray(a);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo.getVertexBufferId());
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.getIndexBufferId());
  }
}

void
PrimitiveRenderer::use(const gpufw::shader_program & program) {
  int id = program.getProgramObjectId();
  if (!id) {
    assert(0);
  } else if (id != current_program) {
    current_program = id;
    glUseProgram(id);
  }
}

void
PrimitiveRenderer::clear() {
  colorMask(true, true, true, true);
  depthMask(true);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
PrimitiveRenderer::colorMask(bool r, bool g, bool b, bool a) {
  if (r != current_red_mask ||
      g != current_green_mask ||
      b != current_blue_mask ||
      a != current_alpha_mask) {
    current_red_mask = r;
    current_green_mask = g;
    current_blue_mask = b;
    current_alpha_mask = a;
    glColorMask( r ? GL_TRUE : GL_FALSE,
		 g ? GL_TRUE : GL_FALSE,
		 b ? GL_TRUE : GL_FALSE,
		 a ? GL_TRUE : GL_FALSE
		 );
  }
}


void
PrimitiveRenderer::setCompositionMode(CompositionMode mode) {
  if (mode != current_composition_mode) {
    current_composition_mode = mode;
    switch (mode) {
    case COPY:
      glBlendFunc(GL_ONE, GL_ZERO);
      break;
    case NORMAL:
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      break;
    case MULTIPLY:
      glBlendFunc(GL_ZERO, GL_SRC_COLOR);
      break;
    }
  }
}

void
PrimitiveRenderer::viewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
  if (w != current_display_size.x || h != current_display_size.y) {
    current_display_size = glm::ivec2(w, h);
    cerr << "updating viewport\n";
    glViewport(0, 0, (unsigned int)(w * display_scale), (unsigned int)(h * display_scale));
  }
}
