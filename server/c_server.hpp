#ifndef ANTICHAT_SERVER_C_SERVER_HPP
#define ANTICHAT_SERVER_C_SERVER_HPP
#include <string>
#include <chrono>
#include "c_room.hpp"
#include "c_network_api.hpp"
#include <thread>
#include <list>
#include "../external/log.hpp"

using std::string;
using std::thread;
using std::mutex;
using std::list;

class c_server {
private:
		static list<c_room> rooms;
		static c_network_api network_api;
		thread sending_thread;
		thread recieving_thread;

		static void handle_recieve ();

		static void start_recieving ();

		static bool is_command (const string &);

		static void start_room (const string &);

		static void send_from (const ClientMessage &);

		static

public:
		c_server ();
};


#endif //ANTICHAT_SERVER_C_SERVER_HPP
