#pragma once

struct pos_t {
	float x, y;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(pos_t, x, y);
};

struct entity_data_t {
	int health = 0;
	int team = 0;
	bool local = false;

	pos_t position = {};
	std::string name = "";
	std::string weapon = "";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(entity_data_t, health, local, name, position, team, weapon);
};