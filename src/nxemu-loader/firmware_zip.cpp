#include "firmware_zip.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "ioapi.h"
#include "unzip.h"

#ifdef _WIN32
#include "iowin32.h"
#endif

namespace
{

constexpr int kWriteBufferSize = 8192;

bool IsZipDirectoryEntry(const char * name)
{
    const size_t length = std::strlen(name);
    return length > 0 && (name[length - 1] == '/' || name[length - 1] == '\\');
}

unzFile OpenZipArchive(const std::filesystem::path & zip_path)
{
#ifdef _WIN32
    zlib_filefunc64_def file_functions{};
    fill_win32_filefunc64A(&file_functions);
    return unzOpen2_64(zip_path.string().c_str(), &file_functions);
#else
    return unzOpen64(zip_path.string().c_str());
#endif
}

bool ExtractCurrentZipEntry(unzFile zip, const std::filesystem::path & output_path)
{
    if (unzOpenCurrentFile(zip) != UNZ_OK)
    {
        return false;
    }

    std::ofstream output(output_path, std::ios::binary);
    if (!output)
    {
        unzCloseCurrentFile(zip);
        return false;
    }

    std::vector<char> buffer(kWriteBufferSize);
    bool success = true;
    for (;;)
    {
        const int bytes_read = unzReadCurrentFile(zip, buffer.data(), static_cast<unsigned>(buffer.size()));
        if (bytes_read == 0)
        {
            break;
        }
        if (bytes_read < 0)
        {
            success = false;
            break;
        }
        output.write(buffer.data(), bytes_read);
        if (!output)
        {
            success = false;
            break;
        }
    }

    unzCloseCurrentFile(zip);
    return success && output.good();
}

} // namespace

bool ExtractFirmwareZipToDirectory(const std::filesystem::path & zip_path, const std::filesystem::path & destination_directory)
{
    std::error_code ec;
    std::filesystem::create_directories(destination_directory, ec);
    if (ec)
    {
        return false;
    }

    unzFile zip = OpenZipArchive(zip_path);
    if (zip == nullptr)
    {
        return false;
    }

    bool success = true;
    if (unzGoToFirstFile(zip) == UNZ_OK)
    {
        do
        {
            char entry_name[512]{};
            unz_file_info64 file_info{};
            if (unzGetCurrentFileInfo64(zip, &file_info, entry_name, sizeof(entry_name), nullptr, 0, nullptr, 0) != UNZ_OK)
            {
                success = false;
                break;
            }

            const std::filesystem::path output_path = destination_directory / entry_name;
            if (IsZipDirectoryEntry(entry_name))
            {
                std::filesystem::create_directories(output_path, ec);
                if (ec)
                {
                    success = false;
                    break;
                }
                continue;
            }

            const std::filesystem::path parent_path = output_path.parent_path();
            if (!parent_path.empty())
            {
                std::filesystem::create_directories(parent_path, ec);
                if (ec)
                {
                    success = false;
                    break;
                }
            }

            if (!ExtractCurrentZipEntry(zip, output_path))
            {
                success = false;
                break;
            }
        } while (unzGoToNextFile(zip) == UNZ_OK);
    }

    unzClose(zip);
    return success;
}
