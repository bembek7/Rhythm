#pragma once
#include <comdef.h>
#include <filesystem>

#define CHECK_HR(hr) \
    if (FAILED(hr)) { \
        _com_error err(hr); \
        const std::filesystem::path filePath(__FILE__); \
        std::stringstream ss; \
        ss << err.ErrorMessage() << std::endl << filePath.filename() << " " << __LINE__; \
        throw std::runtime_error(ss.str()); \
    }