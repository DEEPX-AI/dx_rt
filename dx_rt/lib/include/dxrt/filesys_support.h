#include<string>

namespace dxrt{
    std::string getPath(const std::string& path);
    std::string getCurrentPath();
    std::string getAbsolutePath(const std::string& path);
    std::string getParentPath(const std::string& path);
    int getFileSize(const std::string& filename);
    bool fileExists(const std::string& path);
    std::string getExtension(const std::string& path);
}