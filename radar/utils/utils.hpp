#pragma once

#include <string>
#include <memory>

class Utils
{
public:
	auto wstring_to_string(const std::wstring& wstring) -> std::string;
	auto string_to_wstring(const std::string& string) -> std::wstring;
	auto find_in_string(const std::string& string, const std::string& find) -> bool;
};

inline std::unique_ptr<Utils> utils = std::make_unique<Utils>();