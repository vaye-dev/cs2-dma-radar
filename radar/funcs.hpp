#pragma once
inline void get_first_entity(uint64_t entity, uint64_t& first_entity) {
	if (!entity)
		return;

	const auto entity_identity = process->read<uint64_t>(entity + 0x10);
	if (!entity_identity)
		return;

	uint64_t prev_class = process->read<uint64_t>(process->read<uint64_t>(entity_identity + 0x68));
	if (!prev_class)
		return;

	first_entity = prev_class;
	get_first_entity(prev_class, first_entity);
}

inline uint64_t get_next_entity(uint64_t entity_identity) {
	return process->read<uint64_t>(process->read<uint64_t>(entity_identity + 0x70));
}

inline uint64_t get_entity(uint64_t i) {
	const auto list_entry = process->read<uint64_t>(offset::entity_list + (8 * (i & 0x7FFF) >> 9) + 16);
	if (!list_entry)
		return 0;

	return process->read<uint64_t>((list_entry + 120 * (i & 0x1FF)));
}

inline uint64_t get_entity_pawn(uint64_t entity_pawn_handle) {
	std::uint64_t ListEntry = process->read<uint64_t>(offset::entity_list + 0x8 * ((entity_pawn_handle & 0x7FFF) >> 9) + 16);
	if (!ListEntry)
		return 0;

	return process->read<uint64_t>(ListEntry + 120 * (entity_pawn_handle & 0x1FF));
}