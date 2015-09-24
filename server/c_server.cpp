#include "c_server.hpp"

c_network_api c_server::network_api(12321);
list<c_room> c_server::rooms;

c_server::c_server () : recieving_thread(start_recieving) { }

void c_server::handle_recieve () {
	auto client_msg = network_api.PopMessage();
	if (is_command(client_msg.msg)) {

	} else {

	}
}

bool c_server::is_command (const string &msg) {
	return (!msg.empty() && msg.at(0) == '/');
}

void c_server::start_recieving () {
	while (true) {
		if (network_api.HasMessages())
			handle_recieve();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void c_server::send_from (const ClientMessage &message) {
//	for (auto &user : rooms.begin()->get_users()) {
//		if (user.remote_endpoint == message.remote_endpoint)
//			continue;
//
//		network_api.S
//	}
	network_api.SendToAllExcept()
}

void c_server::start_room (const string &room_name) {
	rooms.push_back(c_room(room_name));
}
