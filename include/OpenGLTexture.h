#ifndef _OPENGLTEXTURE_H_
#define _OPENGLTEXTURE_H_

#include <Texture.h>

#include <InternalFormat.h>
#include <GL.h>

#include <memory>
#include <vector>
#include <cstddef>

namespace canvas {
  class Image;
  class ImageData;
  class Surface;
};

class OpenGLTexture : public Texture {
 public:
  
 OpenGLTexture(unsigned int _logical_width, unsigned int _logical_height, unsigned int _actual_width, unsigned int _actual_height, canvas::FilterMode _min_filter, canvas::FilterMode _mag_filter, canvas::InternalFormat _internal_format, unsigned int _mipmap_levels = 8)
    : Texture(_logical_width, _logical_height, _actual_width, _actual_height, _min_filter, _mag_filter, _internal_format, _mipmap_levels) { }
  ~OpenGLTexture() {
    if (texture_id) {
      freed_textures.push_back(texture_id);
      total_textures--;
    }
  }
  
  OpenGLTexture(canvas::Surface & surface);
  OpenGLTexture(unsigned int _logical_width, unsigned int _logical_height, const canvas::ImageData & image, canvas::FilterMode min_filter, canvas::FilterMode mag_filter, unsigned int mipmap_levels, canvas::InternalFormat target_format = canvas::NO_FORMAT);
  
  unsigned int getTextureId() const override { return texture_id; }
  
  void updateData(const canvas::ImageData & image, unsigned int x, unsigned int y) override;
  void generateMipmaps() override;
  
  static size_t getNumTextures() { return total_textures; }
  static const std::vector<unsigned int> & getFreedTextures() { return freed_textures; }
  static void releaseTextures();
  static std::unique_ptr<Texture> createTexture(unsigned int _logical_width, unsigned int _logical_height, unsigned int _actual_width, unsigned int _actual_height, canvas::FilterMode min_filter, canvas::FilterMode mag_filter, canvas::InternalFormat _internal_format, unsigned int mipmap_levels = 8);
  static std::unique_ptr<Texture> createTexture(canvas::Surface & surface);
  static std::unique_ptr<Texture> createTexture(canvas::Image & image, canvas::FilterMode min_filter = canvas::NEAREST, canvas::FilterMode mag_filter = canvas::NEAREST, unsigned int mipmap_levels = 1, canvas::InternalFormat target_format = canvas::NO_FORMAT);
  
  static bool hasTexStorage() { return has_tex_storage; }
  static void setHasTexStorage(bool t) { has_tex_storage = t; }
  
 protected:
  void updateTextureData(const canvas::ImageData & image, unsigned int x, unsigned int y);
  
 private:
  unsigned int texture_id = 0;
  bool need_mipmaps = false, is_data_initialized = false;
  
  static size_t total_textures;
  static std::vector<unsigned int> freed_textures;
  static bool has_tex_storage;
};

class OpenGLTextureFactory : public TextureFactory {
 public:
  OpenGLTextureFactory() { }
  
};

#endif
