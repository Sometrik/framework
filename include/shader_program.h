#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

#include "GL.h"

#include <glm/glm.hpp>
#include <string>
#include <set>
#include <unordered_map>

namespace gpufw {
  class shader_program {
  public:
    shader_program();
    shader_program(const shader_program & other) = delete;
    shader_program & operator=(const shader_program & other) = delete;

    bool loadShaders(const std::string & glsl_version, const std::string & filename) { return loadShaders(glsl_version.c_str(), filename.c_str()); }
    bool loadShaders(const char * glsl_version, const char * filename);
    bool loadShaderFromFile(GLenum type, const char * glsl_version, const char * filename);
    bool loadShader(GLenum type, const char * glsl_version, const char * shaderSrc, const char * filename = 0);
    
    bool link();
    
    void bindAttribLocation(unsigned int index, const char * name);
    bool isValid() const { return programObject != 0; }
    
    void setUniform(const std::string & name, const glm::mat4 & mat);
    void setUniform(const std::string & name, int s);
    void setUniform(const std::string & name, const glm::vec4 & v);
    void setUniform(const std::string & name, const glm::vec3 & v);
    void setUniform(const std::string & name, const glm::ivec2 & v);
    void setUniform(const std::string & name, float f);
    
    void define(const char * s) { defines.insert(s); }

    unsigned int getProgramObjectId() const { return programObject; }
    
  protected:
    int getUniformLocation(const std::string & name);

  private:
    unsigned int programObject = 0;
    unsigned int vertexShader = 0;
    unsigned int fragmentShader = 0;   
    std::unordered_map<std::string, int> uniform_locations;
    std::set<std::string> defines;
  };
};

#endif
