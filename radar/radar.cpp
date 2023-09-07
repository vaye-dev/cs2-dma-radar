#define _CRT_SECURE_NO_WARNINGS 1
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS 1
#define CS2
#pragma warning(disable : 4996)

#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

#include "utils/process/process.hpp"
#include "utils/json.hpp"
#include "offsets.hpp"
#include "weapon_ids.hpp"
#include "funcs.hpp"
#include "structs.hpp"

HANDLE pipe = INVALID_HANDLE_VALUE;
char pipe_input[1023];
char pipe_output[1023];

std::string map_name = "";

bool connect() {
	pipe = INVALID_HANDLE_VALUE;
	memset(pipe_input, 0, sizeof(pipe_input));
	memset(pipe_output, 0, sizeof(pipe_output));
	
	pipe = CreateNamedPipeA("\\\\.\\pipe\\23d339ddef636cb0a5b9d0be60a289bc4ae87cc62cfd12b8f322e6310c1eea66",
		PIPE_ACCESS_OUTBOUND, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		sizeof(pipe_output), sizeof(pipe_input),
		NULL,
		nullptr);

	if (pipe == INVALID_HANDLE_VALUE)
		std::cout << "pipe INVALID_HANDLE_VALUE = " << GetLastError() << std::endl;

	if (auto pipe_connect = ConnectNamedPipe(pipe, nullptr); !pipe_connect)
		std::cout << "pipe_connect false = " << GetLastError() << std::endl;

	return true;
}

#ifdef CS2
void setup_csgo() {
	std::cout << "Waiting for Counter-Strike: Global Offensive..." << std::endl;
	if (!process->init("cs2.exe"))
		return;

	std::cout << "Found CS2.\n";

	while (!process->client()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (!process->client())
			process->client() = process->get_module_base(L"client.dll");
	}

	auto client = process->client();

	std::thread([]() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			std::cout << "enter map name (de_dust2, de_mirage, de_overpass, etc.): ";
			std::cin >> map_name;
		}
	}).detach();

	connect();
	while (true) {
		const auto local_player = process->read<uint64_t>(client + offset::localplayer_Offset);
		if (!local_player)
			continue;

		offset::g_dwEntList = process->read<uint64_t>(client + offset::s_dwEntityList_Offset);
		if (!offset::g_dwEntList)
			continue;

		static uint64_t first_player_controller = 0;
		if (first_player_controller == 0 || process->read<int>(local_player + offset::s_dwPawnHealth_Offset) <= 0) {
			for (int i = 0; i < 32; i++) {
				const auto entity = get_entity(i);
				if (!entity)
					continue;

				const auto entity_identity = process->read<uint64_t>(entity + 0x10);
				if (!entity_identity)
					continue;

				const auto designer_ptr = process->read<uint64_t>(entity_identity + 0x20);
				if (!designer_ptr)
					continue;

				const auto designer_name = process->read_string(designer_ptr, 32);
				if (designer_name.compare("cs_player_controller"))
					continue;

				get_first_entity(entity, first_player_controller);
				break;
			}
		}

		if (!first_player_controller)
			continue;

		uint64_t player_controller = first_player_controller;
		int i = -1;
		nlohmann::json j;
		j["global"]["map"] = map_name;
		j["global"]["team"] = process->read<int>(local_player + offset::s_teamnum_Offset);
		for (; player_controller; player_controller = get_next_entity(process->read<uint64_t>(player_controller + 0x10))) {
			i++;
			const auto pawn = get_entity_pawn(process->read<uint64_t>(player_controller + offset::s_dwPlayerPawn_Offset));
			if (!pawn)
				continue;

			const auto health = process->read<int>(player_controller + offset::s_dwPawnHealth_Offset);
			if (health <= 0 || health > 100)
				continue;

			const auto name_ptr = process->read<uint64_t>(player_controller + 0x720);
			if (!name_ptr)
				continue;

			auto pos = process->read<pos_t>(pawn + offset::s_Position_Offset);
			auto _i = std::to_string(i);
			j[_i]["health"] = health;
			j[_i]["local"] = process->read<bool>(player_controller + offset::s_bIsLocalPlayerController_Offset);
			j[_i]["name"] = process->read_string(name_ptr, 32);
			j[_i]["position"] = { pos.x, pos.y };
			j[_i]["team"] = process->read<int>(player_controller + offset::s_teamnum_Offset);
			j[_i]["weapon"] = "unknown";
		}

		DWORD write_buffer_written_bytes;
		const auto pipe_write = WriteFile(pipe, j.dump().c_str(), j.dump().size(), &write_buffer_written_bytes, nullptr);
		if (!pipe_write) {
			std::cout << "pipe_write false = " << std::dec << GetLastError() << std::endl;
			if (GetLastError() == 232) {
				DisconnectNamedPipe(pipe);
				CloseHandle(pipe);
				connect();
			}
		}

		if (auto pipe_flush = FlushFileBuffers(pipe); !pipe_flush)
			std::cout << "pipe_flush false = " << std::dec << GetLastError() << std::endl;
	}
}

#else // CS2
void setup_csgo() {
	std::cout << "Waiting for Counter-Strike: Global Offensive..." << std::endl;
	if (!process->init("csgo.exe"))
		return;

	std::cout << "Waiting for serverbrowser.dll..." << std::endl;
	while (!process->get_module_base(L"serverbrowser.dll"))
		std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "serverbrowser.dll found!" << std::endl;

	std::cout << "Trying to get necessary modules..." << std::endl;

	while (!process->client() || !process->engine()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (!process->client())
			process->client() = process->get_module_base(L"client.dll");

		if (!process->engine())
			process->engine() = process->get_module_base(L"engine.dll");
	}

	auto client = process->client();
	auto client_size = process->get_module_size(L"client.dll");

	auto engine = process->engine();
	auto engine_size = process->get_module_size(L"engine.dll");

	std::cout << "All necessary modules found!" << std::endl;
	std::cout << "csgo.exe = 0x" << std::hex << process->process_base() << std::endl;
	std::cout << "client.dll = 0x" << std::hex << client << std::endl;
	std::cout << "engine.dll = 0x" << std::hex << engine << std::endl;

	auto local_player_ptr = process->scan(client, client_size, "\x8D\x34\x85\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x8B\x41\x08\x8B\x48\x04\x83\xF9\xFF", "xxx????xx????xxxxxxxxx") + 3;
	local_player_ptr = process->read<uint32_t>(local_player_ptr) + 4;

	auto client_state = process->scan(engine, engine_size, "\xA1\x00\x00\x00\x00\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0", "x????xxxxxxxxxx") + 1;
	client_state = process->read<uint32_t>(client_state);

	auto client_state_map = process->scan(engine, engine_size, "\x05\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xA1", "x????xxxxxxxxx") + 1;
	client_state_map = process->read<uint32_t>(client_state_map);

	auto entity_list = process->scan(client, client_size, "\xBB\x00\x00\x00\x00\x83\xFF\x01\x0F\x8C\x00\x00\x00\x00\x3B\xF8", "x????xxxxx????xx") + 1;
	entity_list = process->read<uint32_t>(entity_list);

	auto radar_base = process->scan(client, client_size, "\xA1\x00\x00\x00\x00\x8B\x0C\xB0\x8B\x01\xFF\x50\x00\x46\x3B\x35\x00\x00\x00\x00\x7C\xEA\x8B\x0D", "x????xxxxxxx?xxx????xxxx") + 1;
	radar_base = process->read<uint32_t>(radar_base);

	auto dormant = 0xED;

	std::cout << "local_player = 0x" << std::hex << local_player_ptr << "(0x" << local_player_ptr - client << ")" << std::endl;
	std::cout << "client_state = 0x" << std::hex << client_state << "(0x" << client_state - engine << ")" << std::endl;
	std::cout << "client_state_map = 0x" << std::hex << client_state_map << std::endl;
	std::cout << "entity_list = 0x" << std::hex << entity_list << "(0x" << entity_list - client << ")" << std::endl;
	std::cout << "radar_base = 0x" << std::hex << radar_base << "(0x" << radar_base - client << ")" << std::endl;
	std::cout << "dormant = 0x" << std::hex << dormant << std::endl;

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (GetAsyncKeyState(VK_END))
			break;

		auto map = process->read_string(process->read<uint32_t>(client_state) + client_state_map, 64);
		if (map.empty())
			continue;

		const auto local_player = process->read<uint32_t>(local_player_ptr);
		if (!local_player)
			continue;

		nlohmann::json j;
		j["global"]["map"] = map.c_str();
		j["global"]["team"] = process->read<int>(local_player + netvars::m_iTeamNum);
		for (auto i = 0; i <= 32; i++) {
			const auto entity = process->read<uint32_t>(entity_list + 0x10 * i);
			if (!entity)
				continue;

			const auto health = process->read<int>(entity + netvars::m_iHealth);
			if (!health)
				continue;

			const auto lifestate = process->read<uint8_t>(entity + netvars::m_lifeState);
			if (lifestate)
				continue;

			const auto bdormant = process->read<uint8_t>(entity + dormant);
			if (bdormant)
				continue;

			const auto active_weapon = process->read<uint32_t>(entity + netvars::m_hActiveWeapon);
			if (!active_weapon)
				continue;

			const auto weapon_entity = process->read<uint32_t>(entity_list + ((active_weapon & 0xFFF) - 1) * 0x10);
			if (!weapon_entity)
				continue;

			auto weapon_index = process->read<short>(weapon_entity + netvars::m_iItemDefinitionIndex);
			if (weapon_index > 64 || weapon_index < 1)
				weapon_index = 42;

			auto radar = process->read<uint32_t>(process->read<uint32_t>(radar_base) + 0x78);
			if (!radar)
				continue;

			auto name = process->read_string(radar + 0x174 * (i + 2) + 0x18, 128);
			if (name.empty())
				continue;

			struct pos_t {
				float x, y;
			} pos = process->read<pos_t>(entity + netvars::m_vecOrigin);

			const auto _i = std::to_string(i);
			j[_i]["health"] = health;
			j[_i]["local"] = local_player == entity;
			j[_i]["name"] = name.c_str();
			j[_i]["position"] = { pos.x, pos.y };
			j[_i]["team"] = process->read<int>(entity + netvars::m_iTeamNum);
			j[_i]["weapon"] = strcmp(weapon_names[weapon_index], "none") ? weapon_names[weapon_index] : "KNIFE";
		}

		DWORD write_buffer_written_bytes;
		const auto pipe_write = WriteFile(pipe, j.dump().c_str(), j.dump().size(), &write_buffer_written_bytes, nullptr);
		if (!pipe_write) {
			std::cout << "pipe_write false = " << std::dec << GetLastError() << std::endl;
			if (GetLastError() == 232) {
				DisconnectNamedPipe(pipe);
				CloseHandle(pipe);
				connect();
			}
		}

		if (auto pipe_flush = FlushFileBuffers(pipe); !pipe_flush)
			std::cout << "pipe_flush false = " << std::dec << GetLastError() << std::endl;
	}
}
#endif

int main() {
	setup_csgo();
	DisconnectNamedPipe(pipe);
	CloseHandle(pipe);
}
