#ifndef ANTICHAT_SERVER_C_SERVER_HPP
#define ANTICHAT_SERVER_C_SERVER_HPP
#include <string>
#include <chrono>
#include "c_room.hpp"
#include "c_network_api.hpp"
#include <thread>
#include <list>
#include <vector>
#include "../external/log.hpp"

using std::string;
using std::thread;
using std::mutex;
using std::list;
using std::vector;

class c_server {
private:
		static list<c_room> rooms;
		static c_network_api network_api;
		static string answer;
		thread sending_thread;
		thread receiving_thread;
		static mutex mtx;

		static void handle_receive ();

		static void start_recieving ();

		static bool is_command (const string &);

		static void start_room (const string &);

		static void join_room (const string &, const c_user &);

		static void set_nickname (const string &, c_user &);

		static void send_msg (const client_message &);

		static void execute_command (const client_message &);

		static vector<string> parse_msg (const string &);

public:
		c_server ();
};


#endif //ANTICHAT_SERVER_C_SERVER_HPP
