#ifndef ANTICHAT_SERVER_C_ROOM_HPP
#define ANTICHAT_SERVER_C_ROOM_HPP
#include "c_user.hpp"
#include <list>
#include <string>
#include <boost/asio.hpp>

using std::list;
using std::string;

class c_room {
private:
		list<c_user> users;
		string room_name;

public:
		c_room (const string &);

		void add_user (const c_user &);

		void remove_user (const c_user &);

		list<c_user> &get_users ();

		c_user &get_user_by_endpoint (const boost::asio::ip::udp::endpoint &remote_endpoint);

		const string &get_room_name () const { return room_name; }

		bool if_user_exists (const c_user &) const;
};


#endif //ANTICHAT_SERVER_C_ROOM_HPP
