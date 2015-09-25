#ifndef NETWORKLIB_NETWORKSERVER
#define NETWORKLIB_NETWORKSERVER

#include "../external/c_statistics.hpp"
#include "../external/c_locked_queue.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <array>
#include <map>
#include <thread>
#include <atomic>
#include <cstdint>

using boost::asio::ip::udp;

struct client_message {
		std::string msg;
		udp::endpoint remote_endpoint;
};

class c_network_api {
public:
		explicit c_network_api (unsigned short local_port);

		~c_network_api ();

		bool has_messages ();

		client_message pop_message ();

		void send (const std::string &message, const udp::endpoint &target);

		const c_statistics &get_statistics () const { return statistics; };

private:
		// Network send/receive stuff
		boost::asio::io_service io_service;
		udp::socket socket;
		udp::endpoint server_endpoint;
		udp::endpoint remote_endpoint;
		std::array<char, 4096> recv_buffer;
		std::thread service_thread;

		void (*handle_remote_error) (const boost::system::error_code &error_code, const udp::endpoint &remote_endpoint);

		// Low-level network functions
		void start_receive ();

		void handle_receive (const boost::system::error_code &error, std::size_t bytes_transferred);

		void run_service ();

		// Incoming messages queue
		c_locked_queue<client_message> incoming_messages;

		c_network_api (const c_network_api &) = delete;

		c_network_api (c_network_api &&) = delete;

		// Statistics
		c_statistics statistics;
};

#endif