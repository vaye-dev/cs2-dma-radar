#pragma once
#include <cstdint>
#include <cstddef>

// 2023-06-07 07:51:01.337255200 UTC
namespace offset {
	// credits: https://github.com/UnnamedZ03/CS2-external-base/blob/main/source/CSSPlayer.hpp
	constexpr DWORD64 entity_list_offset = 0x178FC88;//dwEntityList
	constexpr DWORD64 view_matrix_offset = 0x187DAB0;//dwViewMatrix
	constexpr DWORD64 local_player_offset = 0x17DE508;//dwLocalPlayerController
	constexpr DWORD64 globals_offset = 0x1692EE8;//dwGlobalVars


	//not use   pawn
	constexpr DWORD64 dwLocalPlayerPawn = 0x187CFC8;// dwLocalPlayerPawn
	constexpr DWORD64 m_iHealth = 0x32C; // int32_t
	constexpr DWORD64 m_hPlayerPawn = 0x7BC; // CHandle<C_CSPlayerPawn>
	constexpr DWORD64 m_iszPlayerName = 0x610; // char[128]
	//


	constexpr DWORD64 m_iPawnHealth = 0x7C8;
	constexpr DWORD64 m_hPawn = 0x5DC;
	constexpr DWORD64 m_sSanitizedPlayerName = 0x720;
	constexpr DWORD64 m_vOldOrigin = 0x1214;
	constexpr DWORD64 m_iTeamNum = 0x3bf;
	constexpr DWORD64 m_bIsLocalPlayerController = 0x6A0;

	inline DWORD64 entity_list = 0;
	inline DWORD64 client = 0;

	class CGlobalVarsBase {
	public:
		float real_time;
		std::int32_t frame_count;
		std::uint8_t padding_0[0x8];
		std::int32_t max_clients;
		float interval_per_tick;
		std::uint8_t padding_1[0x14];
		float current_time;
		float current_time_2;
		std::uint8_t padding_2[0xC];
		std::int32_t tick_count;
		float interval_per_tick_2;
		std::uint8_t padding_3[0x138];
		std::uint64_t current_map;
		std::uint64_t current_map_name;
	};
}