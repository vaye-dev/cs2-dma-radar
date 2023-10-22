#pragma once
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <charconv>
#include <assert.h>
#include "vmmdll.h"
#include "leechcore.h"

class c_process {
public:
	bool init(const std::string& process_name);

	DWORD get_process_pid(const std::string& process_name);
	DWORD get_module_size(const std::wstring& module_name);
	uintptr_t get_module_base(const std::wstring& module_name);

	template<typename T = std::uint64_t>
	T scan(const std::wstring& module_name, const std::string_view pattern) noexcept {
		constexpr auto pattern_to_bytes = [](const std::string_view pattern) -> std::vector<std::int32_t> {
			std::vector<std::int32_t> bytes;
			for (std::size_t i = 0; i < pattern.size(); ++i) {
				if (pattern[i] == ' ')
					continue;

				if (pattern[i] == '?') {
					bytes.push_back(-1);
					continue;
				}

				if (i + 1 < pattern.size()) {
					std::int32_t value = 0;
					if (const auto [ptr, ec] = std::from_chars(pattern.data() + i, pattern.data() + i + 2, value, 16); ec == std::errc())
						bytes.push_back(value);
					++i;
				}
			}
			return bytes;
		};

		const auto module_base = get_module_base(module_name);
		const auto module_size = get_module_size(module_name);
		const auto module_data = std::make_unique<std::uint8_t[]>(module_size);
		if (!read(module_base, module_data.get(), module_size))
			return 0;

		const std::vector<std::int32_t> pattern_bytes = pattern_to_bytes(pattern);
		for (std::size_t i = 0; i < module_size - pattern.size(); ++i) {
			bool found = true;
			for (std::size_t j = 0; j < pattern_bytes.size(); ++j) {
				if (module_data[i + j] != pattern_bytes[j] && pattern_bytes[j] != -1) {
					found = false;
					break;
				}
			}

			if (found)
				return module_base + i;
		}

		return 0;
	}

	template<typename T = std::uint64_t>
	T rip_rel(const uintptr_t address) noexcept {
		auto displacement = read<int32_t>(address + 0x3);
		return address + displacement + 0x7;
	}

	void dump(const std::string& file_name);

	bool read(uintptr_t address, void* buffer, size_t length) {
		return VMMDLL_MemReadEx(m_vmh, (DWORD)m_pid, address, (PBYTE)buffer, (DWORD)length, 0, VMMDLL_FLAG_NOCACHE);
	}

	bool write(uintptr_t address, void* buffer, size_t length) {
		return VMMDLL_MemWrite(m_vmh, (DWORD)m_pid, address, (PBYTE)buffer, (DWORD)length);
	}

	template<typename T>
	T read(uintptr_t address) {
		T buffer{};
		VMMDLL_MemReadEx(m_vmh, (DWORD)m_pid, address, (PBYTE)&buffer, (DWORD)sizeof(T), 0, VMMDLL_FLAG_NOCACHE);
		assert(buffer != NULL);
		return buffer;
	}

	template<typename T>
	size_t write(uintptr_t address, T value) {
		size_t written = 0;
		VMMDLL_MemWrite(m_vmh, (DWORD)m_pid, address, (PBYTE)&value, (DWORD)sizeof(T));
		assert(written != NULL);
		return written;
	}

	auto read_string(const uintptr_t addr, const int max_length = 1000) -> std::string {
		std::string str;
		std::vector<char> chars(max_length);
		if (read(addr, chars.data(), max_length)) {
			for (size_t i = 0; i < chars.size(); i++) {
				if (chars[i] == '\0') break;
				str.push_back(chars[i]);
			}

			if (static_cast<int>(str[0]) == 0 && str.size() == 1) return "";
		}
		return str;
	}

	uintptr_t& client() { return m_client; }
	uintptr_t& engine() { return m_engine; }
	uintptr_t process_base() { return m_base; }
	DWORD pid() { return m_pid; }
private:
	VMM_HANDLE m_vmh = 0;
	DWORD m_pid = 0;
	uintptr_t m_base = 0;
	uintptr_t m_client = 0;
	uintptr_t m_engine = 0;
};

inline std::unique_ptr<c_process> process = std::make_unique<c_process>();