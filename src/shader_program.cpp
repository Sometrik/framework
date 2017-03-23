#include "GL.h"

#include "shader_program.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

#include <glm/gtc/type_ptr.hpp>

#ifdef __ANDROID__
#include "android_fopen.h"
#include <android/log.h>
#endif

using namespace std;
using namespace gpufw;

shader_program::shader_program() { }

bool
shader_program::loadShaders(const char * glsl_version, const char * filename) {
  string shader_text;
#ifdef __ANDROID__
  FILE * in = android_fopen(filename, "r");
  while (!feof(in)) {
    char b[256];
    size_t n = fread(b, 1, 256, in);
    shader_text += string(b, n);
  }
  fclose(in);
#else
  std::ifstream t(filename);
  std::stringstream buffer;
  buffer << t.rdbuf();
  shader_text = buffer.str();
#endif
  
  bool r = true;
  r = loadShader(GL_VERTEX_SHADER, glsl_version, shader_text.c_str(), filename);
  if (r) {
    r = loadShader(GL_FRAGMENT_SHADER, glsl_version, shader_text.c_str(), filename);
  }

#ifdef __ANDROID__
  if (r){
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Shader code compiled succesfully");
  } else {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Shader code compile failed");
  }
#endif
  return r;
}

bool
shader_program::loadShaderFromFile(GLenum type, const char * glsl_version, const char * filename) {
  std::ifstream t(filename);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return loadShader(type, glsl_version, buffer.str().c_str(), filename);
}

bool
shader_program::loadShader(GLenum type, const char * glsl_version, const char * shaderSrc, const char * filename) {
  GLuint shader;
  GLint compiled;

  // Create the shader object
  shader = glCreateShader( type );
  assert(shader != 0);
  if (shader == 0) {
    return 0;
  }

  const char * strings[64]; // 4 + 2 * defines.size()];
  unsigned int si = 0;
  strings[si++] = glsl_version;
  strings[si++] = "\n";
  if (type == GL_VERTEX_SHADER) {
    strings[si++] = "#define VERTEX\n";
  } else if (type == GL_FRAGMENT_SHADER) {
    strings[si++] = "#define FRAGMENT\n";
  }
  for (auto & d : defines) {
    strings[si++] = d.c_str();
    strings[si++] = "\n";
  }
  strings[si++] = "#line 1\n";
  strings[si++] = shaderSrc;

  // Load the shader source
  glShaderSource( shader, 5 + 2 * defines.size(), strings, NULL);
  
  // Compile the shader
  glCompileShader( shader );
  
  // Check the compile status
  glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
  
  if (!compiled) {
    GLint infoLen = 0;
    
    glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

    string infoLog;
    if (infoLen > 1) {
      char * tmp = new char[infoLen + 1];
      glGetShaderInfoLog ( shader, infoLen, NULL, tmp );
      tmp[infoLen] = 0;
      infoLog = tmp;
      delete[] tmp;
    }
     
    string msg = "Error compiling ";
    msg += (type == GL_VERTEX_SHADER ? "vertex" : "fragment" );
    msg += " shader (";
    if (filename) msg += filename;
    msg += "):\n";
    msg += infoLog;

#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "ERROR SHADER COMPILE. COMPILE MESSAGE: %s", msg.c_str());
#else
    cerr << msg << endl;
#endif
      
    glDeleteShader ( shader );
    return false;
  }

  switch (type) {
  case GL_VERTEX_SHADER:
    vertexShader = shader;
    break;
  case GL_FRAGMENT_SHADER:
    fragmentShader = shader;
    break;
  }

  return true;
}

bool
shader_program::link() {
  if (!programObject) programObject = glCreateProgram();  
  if (vertexShader) glAttachShader(programObject, vertexShader);
  if (fragmentShader) glAttachShader(programObject, fragmentShader);

  // Link the program
  glLinkProgram( programObject );

  // Check the link status
  GLint linked;
  glGetProgramiv( programObject, GL_LINK_STATUS, &linked );

  if ( !linked ) {
    GLint infoLen = 0;
    
    glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
    
    string infoLog;
    if ( infoLen > 1 ) {
      char * tmp = new char[infoLen + 1];    
      glGetProgramInfoLog( programObject, infoLen, NULL, tmp );
      tmp[infoLen] = 0;
      infoLog = tmp;      
      delete[] tmp;
    }

    
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "SHADER PROGRAM LINK ERROR. ERROR MESSAGE: %s", infoLog.c_str());
#else
    cerr << "Error linking program:\n" << infoLog << endl;
#endif

    glDeleteProgram ( programObject );
    return false;
  }

  if (vertexShader) glDetachShader(programObject, vertexShader);
  if (fragmentShader) glDetachShader(programObject, fragmentShader);

  return true;
}

void
shader_program::bindAttribLocation(unsigned int index, const char * name) {
  if (!programObject) programObject = glCreateProgram();  
  glBindAttribLocation(programObject, index, name);
}

int
shader_program::getUniformLocation(const std::string & name) {
  auto it = uniform_locations.find(name);
  if (it != uniform_locations.end()) {
    return it->second;
  } else {
    cerr << "getUniformLocation(" << name << "): n = " << uniform_locations.size() << endl;
    if (!programObject) programObject = glCreateProgram();  
    int loc = glGetUniformLocation(programObject, name.c_str());
    uniform_locations[name] = loc;
    return loc;
  }
}

void
shader_program::setUniform(const std::string & name, const glm::mat4 & mat) {
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void
shader_program::setUniform(const std::string & name, int s) {
  glUniform1i(getUniformLocation(name), s);
}

void
shader_program::setUniform(const std::string & name, const glm::vec4 & v) {
  glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void
shader_program::setUniform(const std::string & name, const glm::vec3 & v) {
  glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void
shader_program::setUniform(const std::string & name, const glm::ivec2 & v) {
  glUniform2iv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void
shader_program::setUniform(const std::string & name, float f) {
  glUniform1f(getUniformLocation(name), f);
}
