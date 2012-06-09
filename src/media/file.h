#ifndef MEDIA_FILE_H
#define MEDIA_FILE_H

#include <string>

class File{

    public:
        File(const std::string &nPath, const std::string &nFilename)
            { _path = nPath; _filename = nFilename; }

        std::string path() const { return _path; }
        std::string filename() const { return _filename; }
        std::string mimetype() const;

        File setBaseName(const std::string &name);

    private:
        std::string _path;
        std::string _filename;
};

#endif // MEDIA_FILE_H
