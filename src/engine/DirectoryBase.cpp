#include "DirectoryBase.hpp"
#include "ScaleSize.hpp"

const MyString DirectoryBase::GFX_OBJECT_SET_EXTENSION = ".mos";
const MyString DirectoryBase::LEVEL_EXTENSION = ".mlv";

/*static */
MyString DirectoryBase::get_root() {
  // native mode
  return "resource";
}

MyString DirectoryBase::get_sound_path() { return get_root() / "../sound"; }

MyString DirectoryBase::get_size_dir() { return "1x"; }

MyString DirectoryBase::get_tiles_path(bool images) {
  MyString rval = get_root() / "tiles";
  if (images) {
    rval = rval / get_size_dir();
  }
  return rval;
}

MyString DirectoryBase::get_images_extension() { return ".bmp"; }
MyString DirectoryBase::get_sound_extension() { return ".wav"; }

MyString DirectoryBase::get_sprites_path() {
  return get_root() / get_size_dir() / "sprites";
}
MyString DirectoryBase::get_images_path() {
  return get_root() / get_size_dir() / "images";
}
MyString DirectoryBase::rework_path(const MyString &path,
                                    const MyString &leading_to_remove,
                                    bool remove_root, bool remove_extension) {
  MyString rval = path;

  if (remove_root) {
    rval = path.replace(leading_to_remove, "");
  }
  if (remove_extension) {
    int idx = rval.find_last_of(".");
    if (idx != -1) {
      rval = rval.substr(0, idx);
    }
  }

  return rval;
}
