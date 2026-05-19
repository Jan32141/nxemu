#pragma once

#include <filesystem>

bool ExtractFirmwareZipToDirectory(const std::filesystem::path & zip_path,
                                   const std::filesystem::path & destination_directory);
