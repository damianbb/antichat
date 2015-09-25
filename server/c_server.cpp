#include "c_server.hpp"

c_network_api c_server::network_api(12321);
list<c_room> c_server::rooms;
string c_server::answer;
mutex c_server::mtx;

c_server::c_server () : recieving_thread(start_recieving) {
	recieving_thread.join();
}

void c_server::handle_recieve () {
	auto client_msg = network_api.pop_message();
	if (is_command(client_msg.msg)) {
		mtx.lock();
		execute_command(client_msg);
		network_api.send(answer, client_msg.remote_endpoint);
		mtx.unlock();

	} else {
		send_msg(client_msg);
	}
}

bool c_server::is_command (const string &msg) {
	return (!msg.empty() && msg.at(0) == '/');
}

void c_server::start_recieving () {
	while (true) {
		if (network_api.has_messages())
			handle_recieve();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void c_server::send_msg (const client_message &message) {
	const c_user &sender = rooms.begin()->get_user_by_endpoint(message.remote_endpoint);
	const string msg_to_send = sender.nickname + ": " + message.msg;

	for (const c_user &user : rooms.begin()->get_users()) {
		if (sender == user)
			continue;

		network_api.send(msg_to_send, user.remote_endpoint);
	}
}

void c_server::start_room (const string &room_name) {
	rooms.push_back(c_room(room_name));
	answer = "successful!";
}

void c_server::execute_command (const client_message &message) {
	auto parsed_command = parse_msg(message.msg);

	if (parsed_command.at(0) == "/start") {
		start_room(parsed_command.at(1));

	} else if (parsed_command.at(0) == "/join") {
		c_user user;
		user.remote_endpoint = message.remote_endpoint;
		join_room(parsed_command.at(1), user);

	} else if (parsed_command.at(0) == "/nick") {
		try {
			c_user &sender = rooms.begin()->get_user_by_endpoint(message.remote_endpoint);
			sender.nickname = parsed_command.at(1);
			answer = "successful!";
		} catch (std::runtime_error &er) {
			answer = string(er.what());
		}

	} else {
		answer = "no such command";
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
