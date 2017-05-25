#include "GL.h"

#include "shader_program.h"

#include <iostream>
#include <cassert>

#include <glm/gtc/type_ptr.hpp>

#ifdef __ANDROID__
#include <android/log.h>
#endif

using namespace std;

bool
shader_program::loadShaders(const std::string & glsl_version, const std::string & shaderSrc) {
  return
    loadShader(GL_VERTEX_SHADER, glsl_version, shaderSrc) &&
    loadShader(GL_FRAGMENT_SHADER, glsl_version, shaderSrc);
}

bool
shader_program::loadShader(GLenum type, const std::string & glsl_version, const std::string & shaderSrc) {
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
  strings[si++] = glsl_version.c_str();
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
  strings[si++] = shaderSrc.c_str();

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
    msg += " shader:\n";
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
