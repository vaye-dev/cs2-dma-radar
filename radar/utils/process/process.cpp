#include "process.hpp"

#include <cstdio>
#include <wchar.h>
#include <tchar.h>
#include <assert.h>
#include <iostream>

#include "../utils.hpp"

bool c_process::init(const std::string& process_name) {
	LPSTR args[] = { (LPSTR)"", (LPSTR)"-device", (LPSTR)"fpga" };
	m_vmh = VMMDLL_Initialize(3, args);
	if (!m_vmh) {
		std::cout << "Failed initializing vmdll.\n";
		return false;
	}

	if (m_pid = get_process_pid(process_name); !m_pid) {
		std::cout << "Failed opening process handle.\n";
		return false;
	}

	m_base = get_module_base(utils->string_to_wstring(process_name));
	if (!m_base) {
		std::cout << "Failed getting base address for process.\n";
		return false;
	}

	return true;
}

DWORD c_process::get_process_pid(const std::string& process_name) {
	DWORD pid = 0;
	if (!VMMDLL_PidGetFromName(m_vmh, (LPSTR)process_name.c_str(), &pid))
		return 0;

	return pid;
}

uintptr_t c_process::get_module_base(const std::wstring& module_name) {
	return VMMDLL_ProcessGetModuleBase(m_vmh, (DWORD)m_pid, (LPWSTR)module_name.c_str());
}

DWORD c_process::get_module_size(const std::wstring& module_name) {
	PVMMDLL_MAP_MODULEENTRY me;
	if (!VMMDLL_Map_GetModuleFromName(m_vmh, (DWORD)m_pid, (LPWSTR)module_name.c_str(), &me, VMMDLL_MODULE_FLAG_NORMAL)) {
		return 0;
	}

	return me->cbImageSize;
}

void c_process::dump(const std::string& file_name) {
	auto base_address = m_base;
	auto dos_header = read<IMAGE_DOS_HEADER>(base_address);
	auto nt_headers = read<IMAGE_NT_HEADERS>(base_address + dos_header.e_lfanew);

	if (dos_header.e_magic != IMAGE_DOS_SIGNATURE || nt_headers.Signature != IMAGE_NT_SIGNATURE || nt_headers.OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC)
		return;

	// allocate size of image to heap
	auto target = std::unique_ptr<uint8_t[]>(new uint8_t[nt_headers.OptionalHeader.SizeOfImage]);

	// fill buffer
	read(base_address, target.get(), nt_headers.OptionalHeader.SizeOfImage);

	auto pnt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(target.get() + dos_header.e_lfanew);
	auto section_headers = reinterpret_cast<PIMAGE_SECTION_HEADER>(
		target.get() +
		static_cast<size_t>(dos_header.e_lfanew) +
		static_cast<size_t>(FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader)) +
		static_cast<size_t>(nt_headers.FileHeader.SizeOfOptionalHeader));

	// fix section headers
	for (size_t i = 0; i < nt_headers.FileHeader.NumberOfSections; i += 1) {
		auto& sec = section_headers[i];
		sec.PointerToRawData = sec.VirtualAddress;
		sec.SizeOfRawData = sec.Misc.VirtualSize;
		if (!memcmp(sec.Name, ".reloc\0\0", 8)) {
			pnt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC] = {
				sec.VirtualAddress,
				sec.Misc.VirtualSize,
			};
		}
	}

	// write dump to file
	const auto dump_file = CreateFileA(file_name.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_COMPRESSED, NULL);
	if (dump_file != INVALID_HANDLE_VALUE) {
		WriteFile(dump_file, target.get(), static_cast<uint32_t>(nt_headers.OptionalHeader.SizeOfImage), NULL, NULL);
		CloseHandle(dump_file);
	}
}