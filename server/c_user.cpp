#include "c_user.hpp"

bool operator== (const c_user &lhs, const c_user &rhs) {
	return (lhs.nickname == rhs.nickname || lhs.remote_endpoint == rhs.remote_endpoint);
}

bool operator!= (const c_user &lhs, const c_user &rhs) {
	return !(lhs == rhs);
}