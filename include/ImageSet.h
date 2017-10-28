#ifndef _IMAGESET_H_
#define _IMAGESET_H_

#include <string>
#include <set>
#include <cassert>

struct image_url_s {
  bool operator< (const image_url_s & other) const {
    if (width < other.width) return true;
    else if (width == other.width && height < other.height) return true;
    return false;
  }

  unsigned short width, height;
  std::string url;
};

class ImageSet {
 public:
  ImageSet() = default;

  const image_url_s & getSmallest() const {
    if (images.empty()) {
      return null_image;
    } else {
      auto it = images.begin();
      return *it;
    }    
  }

  const image_url_s & getLargest() const {
    if (images.empty()) {
      return null_image;
    } else {
      auto it = images.end();
      it--;
      return *it;
    }
  }

  const image_url_s & getSuitable(unsigned int target_width) const {
    for (auto & i : images) {
      if (4 * (unsigned int)i.width >= 3 * target_width) { // select image if it's at least 0.75 * target_width
	return i;
      }
    }
    return getLargest();
  }

  void insert(const image_url_s & image) { images.insert(image); }
  void insert(const std::string & url) { insert(0, 0, url); }
  void insert(unsigned short w, unsigned short h, const std::string & url) {
    image_url_s u = { w, h, url };
    insert(u);
  }

  bool empty() const { return images.empty(); }
  void clear() { images.clear(); }
  const std::set<image_url_s> & data() const { return images; }
  
 private:
  std::set<image_url_s> images;
  image_url_s null_image;
};

#endif
