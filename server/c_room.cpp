#include "c_room.hpp"

void c_room::add_user (const c_user &user) {
	users.push_back(user);
}

void c_room::remove_user (const c_user &user) {
	users.remove(user);
}

bool c_room::if_user_exists (const c_user &user) const {
	for (const c_user &u : users) {
		if (u == user)
			return true;
	}

	return false;
}

list<c_user> &c_room::get_users () {
	return users;
}

c_room::c_room (const string &room_name) : room_name(room_name) { }
