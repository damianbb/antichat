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

typedef std::map<uint32_t, udp::endpoint> ClientList;
typedef ClientList::value_type Client;

struct ClientMessage {
		std::string msg;
		udp::endpoint remote_endpoint;
		int32_t client_id; // strongly linked with session!
};

class c_network_api {
public:
		explicit c_network_api (unsigned short local_port);

		~c_network_api ();

		bool HasMessages ();

		ClientMessage PopMessage ();

		void SendToClient (const std::string &message, uint32_t clientID);

		void SendToAllExcept (const std::string &message, uint32_t clientID);

		void SendToAll (const std::string &message);

		const c_statistics &GetStatistics () const { return statistics; };

		std::vector<std::function<void (int32_t)>> clientDisconnectedHandlers;

private:
		// Network send/receive stuff
		boost::asio::io_service io_service;
		udp::socket socket;
		udp::endpoint server_endpoint;
		udp::endpoint remote_endpoint;
		std::array<char, 4096> recv_buffer;
		std::thread service_thread;

		// Low-level network functions
		void start_receive ();

		void handle_remote_error (const boost::system::error_code &error_code, const udp::endpoint remote_endpoint);

		void handle_receive (const boost::system::error_code &error, std::size_t bytes_transferred);

		void handle_send (std::string /*message*/, const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/) { }

		void run_service ();

		// Client management
		int32_t get_or_create_client_id (udp::endpoint endpoint);

		void on_client_disconnected (int32_t id);

		void send (const std::string &message, udp::endpoint target);

		// Incoming messages queue
		c_locked_queue<ClientMessage> incomingMessages;

		// Clients of the server
		ClientList clients;
		std::atomic_int_fast32_t nextClientID;

		c_network_api (c_network_api &) = delete;

		// Statistics
		c_statistics statistics;
};

#endif