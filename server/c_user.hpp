#ifndef ANTICHAT_SERVER_C_USER_HPP
#define ANTICHAT_SERVER_C_USER_HPP
#include <string>
#include <boost/asio.hpp>

using std::string;

class c_user {
public:
		string nickname;
		boost::asio::ip::udp::endpoint remote_endpoint;
		uint32_t current_client_id;

		c_user (const string & = "unknown");
};

bool operator== (const c_user &lhs, const c_user &rhs);
bool operator!= (const c_user &lhs, const c_user &rhs);


#endif //ANTICHAT_SERVER_C_USER_HPP
