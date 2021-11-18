#ifndef GRAPH3D_UTIL_RESOURCES_H_
#define GRAPH3D_UTIL_RESOURCES_H_

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <exceptions/messages.h>
#include <exceptions/warning.h>

namespace graph3d {
namespace util {

enum ResourceType { G3D_RESOURCE_SHADER, G3D_RESOURCE_MODEL, G3D_RESOURCE_TEXTURE };

const std::string getResourceTypeName(ResourceType type) {
  switch (type) {
    case G3D_RESOURCE_MODEL:
      return "Modelo";
    case G3D_RESOURCE_SHADER:
      return "Shader";
    case G3D_RESOURCE_TEXTURE:
      return "Textura";
  }
  return "";
}

std::string operator+(const std::string& str, const std::filesystem::path& path) { return str + path.u8string(); }

class ResourceManager {
 public:
  /// Singleton
  static ResourceManager& getInstance() {
    static ResourceManager instance;
    return instance;
  }

 private:
  std::map<ResourceType, std::vector<std::string>> resourceMaps;

 public:
  void addResourceFolder(ResourceType type, const std::string& folder) {
    auto it = resourceMaps.find(type);

    if (it == resourceMaps.end()) it = resourceMaps.emplace(type, std::vector<std::string>()).first;

    bool path = folder.at(folder.size() - 1) == '/';

    it->second.emplace_back(path ? folder : folder + '/');
  }

  std::filesystem::path getResourceFilePath(ResourceType type, const std::string& filename) {
    std::ifstream file;
    std::vector<std::string> resources;

    if (getResources(type, resources)) {
      for (std::string& folder : resources) {
        if (std::filesystem::exists(folder + filename)) return folder + filename;
        exceptions::warning("WAR002", exceptions::format(exceptions::WAR002, filename));
      }
    }
    return "";
  }

  std::filesystem::path getResourceFolderPath(ResourceType type, const std::string& foldername) {
    std::vector<std::string> resources;
    std::filesystem::path path;

    if (getResources(type, resources)) {
      for (std::string& resource : resources) {
        path = resource + foldername;
        if (std::filesystem::exists(path)) break;
      }

      if (path.empty()) exceptions::warning("WAR003", exceptions::format(exceptions::WAR003, foldername));
    }

    return path;
  }

  std::ifstream getResourceFile(ResourceType type, const std::filesystem::path& filename) {
    std::ifstream file;
    std::vector<std::string> resources;

    if (getResources(type, resources)) {
      for (std::string& folder : resources) {
        file.open(folder + filename);
        if (file.is_open()) break;
        file.clear();
      }

      if (!file.is_open()) exceptions::warning("WAR002", exceptions::format(exceptions::WAR002, filename));
    }

    return file;
  }

  std::ifstream getResourceFile(ResourceType type, std::filesystem::path& filename) {
    std::ifstream file;
    std::vector<std::string> resources;

    if (getResources(type, resources)) {
      for (std::string& folder : resources) {
        file.open(folder + filename);
        if (file.is_open()) {
          filename = folder + filename;
          break;
        }
        file.clear();
      }

      if (!file.is_open()) exceptions::warning("WAR002", exceptions::format(exceptions::WAR002, filename));
    }

    return file;
  }

 private:
  bool getResources(ResourceType type, std::vector<std::string>& resources) {
    auto it = resourceMaps.find(type);

    if (it == resourceMaps.end()) {
      exceptions::warning("WAR001", exceptions::format(exceptions::WAR001, getResourceTypeName(type)));
      return false;
    }

    resources = it->second;
    return true;
  }
};  // namespace util

void addResourceFolder(ResourceType type, const std::string& resourcefolder) {
  return ResourceManager::getInstance().addResourceFolder(type, std::forward<const std::string&>(resourcefolder));
}

std::ifstream getResourceFile(ResourceType type, const std::filesystem::path& filename) {
  return ResourceManager::getInstance().getResourceFile(std::forward<ResourceType>(type), filename);
}

std::ifstream getResourceFile(ResourceType type, std::filesystem::path& filename) {
  return ResourceManager::getInstance().getResourceFile(std::forward<ResourceType>(type),
                                                        std::forward<std::filesystem::path&>(filename));
}

std::filesystem::path getResourceFilePath(ResourceType type, const std::string& filename) {
  return ResourceManager::getInstance().getResourceFilePath(std::forward<ResourceType>(type), filename);
}

std::filesystem::path getResourceFolderPath(ResourceType type, const std::string& foldername) {
  return ResourceManager::getInstance().getResourceFolderPath(std::forward<ResourceType>(type), foldername);
}

}  // namespace util
}  // namespace graph3d

#endif