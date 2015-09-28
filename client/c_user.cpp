#include "c_user.hpp"

c_user::c_user (const string &nickname) : m_nickname(nickname) {
	crypto_api.generate_key();
}

void c_user::sign_msg (const string &msg, unsigned char *Signature) {
	crypto_api.sign(msg, Signature);
}

const unsigned char *const c_user::get_public_key () const {
	return crypto_api.get_public_key();
}
