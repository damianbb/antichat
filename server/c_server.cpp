#include "c_server.hpp"

c_network_api c_server::network_api(12321);
list<c_room> c_server::rooms;
string c_server::answer;

c_server::c_server () : recieving_thread(start_recieving) {
	recieving_thread.join();
}

void c_server::handle_recieve () {
	auto client_msg = network_api.PopMessage();
	if (is_command(client_msg.msg)) {
		execute_command(client_msg);
		network_api.SendToClient(answer, client_msg.client_id);

	} else {
		send_msg(client_msg);
//		answer = "message recieved";
	}
//	network_api.SendToClient(answer, client_msg.client_id);
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

void c_server::send_msg (const ClientMessage &message) {
	network_api.SendToAllExcept(message.msg, message.client_id);
}

void c_server::start_room (const string &room_name) {
	rooms.push_back(c_room(room_name));
	answer = "successful!";
}

void c_server::execute_command (const ClientMessage &message) {
	auto parsed_command = parse_msg(message.msg);

	if (parsed_command.at(0) == "/start") {
		start_room(parsed_command.at(1));

	} else if (parsed_command.at(0) == "/join") {
		c_user user;
		user.remote_endpoint = message.remote_endpoint;
		user.nickname = "temp";
		join_room(parsed_command.at(1), user);
	}
}

vector<string> c_server::parse_msg (const string &message) {
	vector<string> result;
	size_t first = 0, last = message.find(' ');

	do {
		result.push_back(message.substr(first, last - first));
		first = last + 1;
		last = message.find(' ', first);
	} while (last != string::npos);

	result.push_back(message.substr(first));

	return result;
}

void c_server::join_room (const string &room_name, const c_user &user) {
	for (auto &u : rooms) {
		if (u.get_room_name() == room_name) {
			u.add_user(user);
			answer = "successful!";
			return;
		}
	}
	answer = "no such room";
}
