#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <InternalFormat.h>

namespace canvas {
  class ImageData;
};

class Texture {
 public:
  enum FilterMode {
    NEAREST = 1,
    LINEAR,
    LINEAR_MIPMAP_LINEAR
  };

  
 Texture(unsigned int _logical_width, unsigned int _logical_height, unsigned int _actual_width, unsigned int _actual_height, FilterMode _min_filter, FilterMode _mag_filter, canvas::InternalFormat _internal_format, unsigned int _mipmap_levels)
   : logical_width(_logical_width), logical_height(_logical_height),
    actual_width(_actual_width), actual_height(_actual_height),
    mipmap_levels(_mipmap_levels),
    min_filter(_min_filter), mag_filter(_mag_filter),
    internal_format(_internal_format) { }
  Texture(const Texture & other) = delete;
  
  virtual ~Texture() = default;
  
  Texture & operator=(const Texture & other) = delete;

  virtual void updateData(const canvas::ImageData & image, unsigned int x, unsigned int y) = 0;
  virtual void generateMipmaps() { }
  virtual unsigned int getTextureId() const { return 0; }
  
  unsigned int getLogicalWidth() const { return logical_width; }
  unsigned int getLogicalHeight() const { return logical_height; }
  unsigned int getActualWidth() const { return actual_width; }
  unsigned int getActualHeight() const { return actual_height; }
  unsigned int getMipmapLevels() const { return mipmap_levels; }
  FilterMode getMinFilter() const { return min_filter; }
  FilterMode getMagFilter() const { return mag_filter; }
  canvas::InternalFormat getInternalFormat() const { return internal_format; }
  bool isDefined() const { return getTextureId() != 0; }
  
  int getUpdateCursor() const { return update_cursor; }
  void setUpdateCursor(int c) { update_cursor = c; }
  
 private:  
  unsigned int logical_width, logical_height, actual_width, actual_height, mipmap_levels;
  FilterMode min_filter, mag_filter;
  canvas::InternalFormat internal_format;
  int update_cursor = 0;
  };

class TextureFactory {
 public:
  TextureFactory() { }
  
};

#endif
