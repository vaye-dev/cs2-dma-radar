#pragma once
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <assert.h>
#include "vmmdll.h"
#include "leechcore.h"

class c_process {
public:
	bool init(const std::string& process_name);

	DWORD get_process_pid(const std::string& process_name);
	DWORD get_module_size(const std::wstring& module_name);
	uintptr_t get_module_base(const std::wstring& module_name);
	uint32_t scan(uintptr_t start, size_t size, const char* signature, const char* mask);
	
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