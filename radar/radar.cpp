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
	if (auto pipe_connect = ConnectNamedPipe(pipe, nullptr);
	!pipe_connect)
			std::cout << "pipe_connect false = " << GetLastError() << std::endl;
	return true;
}
void setup_csgo() {
	std::cout << "Waiting for Counter-Strike 2..." << std::endl;
	if (!process->init("cs2.exe"))
			return;
	std::cout << "Found CS2.\n";
	while (!process->client()) {
		std::this_thread::sleep_for (std::chrono::milliseconds(50));
		if (!process->client())
					process->client() = process->get_module_base(L"client.dll");
	}
	auto client = process->client();
	connect();
	while (true) {
		auto pglobals = process->read<uint64_t>(offset::globals_offset);
		const auto local_player = process->read<uint64_t>(client + offset::local_player_offset);
		if (!local_player)
					continue;
		const auto globals = process->read<offset::CGlobalVarsBase>(pglobals);
		auto pglobals1 = process->read<uint64_t>(client + offset::globals_offset);
		auto pglobals2 = process->read<uint64_t>(((pglobals1 + 0x188) + 0) + 0);
		const auto map_name = process->read_string(pglobals2, 32);
		if (map_name == "<empty>")
					continue;
		offset::entity_list = process->read<uint64_t>(client + offset::entity_list_offset);
		if (!offset::entity_list)
					continue;
		static uint64_t first_player_controller = 0;
		//if (first_player_controller == 0 || process->read<int>(local_player + offset::m_iHealth) <= 0) {
		if (first_player_controller == 0 || process->read<int>(local_player + offset::m_iPawnHealth) <= 0) {
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
		j["global"]["team"] = process->read<int>(local_player + offset::m_iTeamNum);
		for (; player_controller; player_controller = get_next_entity(process->read<uint64_t>(player_controller + 0x10))) {
			i++;
			const auto pawn = get_entity_pawn(process->read<uint64_t>(player_controller + offset::m_hPawn));
			if (!pawn)
							continue;
			const auto health = process->read<int>(pawn + offset::m_iHealth);
			if (health <= 0 || health > 100)
							continue;
			const auto name_ptr = process->read<uint64_t>(player_controller + 0x720
			/* m_sSanitizedPlayerName?*/
			);
			if (!name_ptr)
							continue;
			auto pos = process->read<pos_t>(pawn + offset::m_vOldOrigin);
			auto _i = std::to_string(i);
			j[_i]["health"] = health;
			j[_i]["local"] = process->read<bool>(player_controller + offset::m_bIsLocalPlayerController);
			j[_i]["name"] = process->read_string(name_ptr, 32);
			j[_i]["position"] = {
				pos.x, pos.y
			}
			;
			j[_i]["team"] = process->read<int>(player_controller + offset::m_iTeamNum);
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
		if (auto pipe_flush = FlushFileBuffers(pipe);
		!pipe_flush)
					std::cout << "pipe_flush false = " << std::dec << GetLastError() << std::endl;
	}
}
int main() {
	setup_csgo();
	DisconnectNamedPipe(pipe);
	CloseHandle(pipe);
}
