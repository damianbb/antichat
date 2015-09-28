#ifndef COIN_SENDING_C_USER_HPP
#define COIN_SENDING_C_USER_HPP
#include <string>
#include <algorithm>
#include "../external/c_crypto_ed25519.hpp"


class c_user {
private:
		std::string m_nickname;
		c_crypto_ed25519 crypto_api;
public:
		c_user (const string &);

		const unsigned char *const get_public_key () const;

		void sign_msg (const string &, unsigned char [64]);
};


#endif //COIN_SENDING_C_USER_HPP
