#pragma once
#include <cstdint>
#include <cstddef>

// 2023-06-07 07:51:01.337255200 UTC
namespace offset {
	// credits: https://github.com/UnnamedZ03/CS2-external-base/blob/main/source/CSSPlayer.hpp
	inline DWORD64 entity_list_sig = 0x0;
	inline DWORD64 local_player = 0x0;
	inline DWORD64 globals = 0x0;

	constexpr DWORD64 m_iHealth = 0x32c;
	constexpr DWORD64 m_hPlayerPawn = 0x7bc;
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