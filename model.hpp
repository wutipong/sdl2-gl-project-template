#pragma once

#include <tiny_gltf.h>

namespace Model {
tinygltf::Model LoadFromFile(const std::string &path);
}
