#include "c_user.hpp"

bool operator== (const c_user &lhs, const c_user &rhs) {
	return (lhs.nickname == rhs.nickname || lhs.remote_endpoint == rhs.remote_endpoint);
}

bool operator!= (const c_user &lhs, const c_user &rhs) {
	return !(lhs == rhs);
}

c_user::c_user (const string &nickname) : nickname(nickname) { }
