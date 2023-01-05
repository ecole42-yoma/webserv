#include "spx_session_storage.hpp"

// temp
#include "spx_client_buffer.hpp"
#include <sys/time.h>

bool
SessionStorage::is_key_exsits(const std::string& c_key) const {
	storage_t::const_iterator it = storage_.find(c_key);
	if (it == storage_.end()) {
		return false;
	}
	return true;
}

session_t&
SessionStorage::find_value_by_key(std::string& c_key) {
	storage_t::iterator it = storage_.find(c_key);
	return it->second;
}
/*
std::string
SessionStorage::find_session_to_string(const std::string& c_key) {
	storage_t::iterator it = storage_.find(c_key);
	if (it == storage_.end())
		return "";
	return it->second.to_string();
}
*/

void
SessionStorage::add_new_session(SessionID id) {
	storage_.insert(session_key_val(id, 0));
}

std::string
SessionStorage::make_hash(uintptr_t& seed_in) {
	struct timeval time;
	gettimeofday(&time, NULL);

	uint32_t t = time.tv_usec;
	std::srand(t);

	uint32_t r = std::rand();

	std::bitset<24> rand_char(r);
	std::bitset<24> time_char(t);

	std::string hash_str;

	for (int i = 0; i < 4; ++i) {
		if (seed_in & 1) {
			hash_str += static_cast<char>('!' + (time_char.to_ulong() & 0x3f));
			hash_str += static_cast<char>('=' + (rand_char.to_ulong() & 0x3f));
		} else {
			hash_str += static_cast<char>('=' + (time_char.to_ulong() & 0x3f));
			hash_str += static_cast<char>('!' + (rand_char.to_ulong() & 0x3f));
		}
		time_char >>= 6;
		rand_char >>= 6;
	}
	return hash_str;
}

void
SessionStorage::addCount() {
	++count;
}

// this code will moved to client_buf file

void
ResField::setSessionHeader(std::string session_id) {
	headers_.push_back(header("Set-Cookie", "sessionID=" + session_id));
}
