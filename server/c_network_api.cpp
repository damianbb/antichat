#include "c_network_api.hpp"
#include "../external/log.hpp"

c_network_api::c_network_api (unsigned short local_port)
	: socket(io_service, udp::endpoint(udp::v6(), local_port)),
		service_thread(&c_network_api::run_service, this),
		nextClientID(0L) {

	Log::Info("Starting server on port ", local_port);
};

c_network_api::~c_network_api () {
	io_service.stop();
	service_thread.join();
}

void c_network_api::start_receive () {
	socket.async_receive_from(boost::asio::buffer(recv_buffer),
														remote_endpoint,
		                        boost::bind(&c_network_api::handle_receive, this,
								                        boost::asio::placeholders::error,
																				boost::asio::placeholders::bytes_transferred));


}

void c_network_api::on_client_disconnected (int32_t id) {
	for (auto &handler : clientDisconnectedHandlers)
		if (handler)
			handler(id);
}

void c_network_api::handle_remote_error (const boost::system::error_code& error_code, const udp::endpoint remote_endpoint) {
	bool found = false;
	int32_t id;
	for (const auto &client : clients)
		if (client.second == remote_endpoint) {
			found = true;
			id = client.first;
			break;
		}
	if (!found)
		return;

	clients.erase(id);
	on_client_disconnected(id);
}

void c_network_api::handle_receive (const boost::system::error_code &error, std::size_t bytes_transferred) {
	if (!error && bytes_transferred < 4096) {
		try {
			ClientMessage message;
			message.msg = std::string(recv_buffer.data(), recv_buffer.data() + bytes_transferred);
			message.remote_endpoint = remote_endpoint;
			message.client_id = get_or_create_client_id(remote_endpoint);

			if (!message.msg.empty())
				incomingMessages.push(message);

			statistics.register_received_message(bytes_transferred);

			Log::Info("recieved msg from ", remote_endpoint.address());
			Log::Info("     message: ", message.msg);
			Log::Info("     length of msg: ", bytes_transferred);
		} catch (std::exception ex) {
			Log::Error("handle_receive: Error parsing incoming message:", ex.what());
		} catch (...) {
			Log::Error("handle_receive: Unknown error while parsing incoming message");
		}

	} else {
		Log::Error("handle_receive: error: ", error.message(), " while receiving from address ", remote_endpoint.address());
		handle_remote_error(error, remote_endpoint);
	}

	start_receive();
}

void c_network_api::send (const std::string &message, udp::endpoint target_endpoint) {
	socket.send_to(boost::asio::buffer(message), target_endpoint);
	statistics.register_sent_message(message.size());
}

void c_network_api::run_service () {
	start_receive();
	while (!io_service.stopped()) {
		try {
			io_service.run();
		} catch (const std::exception &e) {
			Log::Error("NetworkServer: Network exception: ", e.what());
		} catch (...) {
			Log::Error("NetworkServer: Network exception: unknown");
		}
	}
	Log::Debug("Server network thread stopped");
};

int32_t c_network_api::get_or_create_client_id (udp::endpoint endpoint) {
	for (const auto &client : clients)
		if (client.second == endpoint)
			return client.first;

	auto id = ++nextClientID;
	clients.insert(Client(id, endpoint));
	return id;
};

void c_network_api::SendToClient (const std::string &message, uint32_t clientID) {
	try {
		send(message, clients.at(clientID));
	} catch (std::out_of_range) {
		Log::Error(__FUNCTION__, ": Unknown client ID ", clientID);
	}
};

void c_network_api::SendToAllExcept (const std::string &message, uint32_t clientID) {
	for (auto client : clients)
		if (client.first != clientID)
			send(message, client.second);
};

void c_network_api::SendToAll (const std::string &message) {
	for (auto client : clients)
		send(message, client.second);
};

ClientMessage c_network_api::PopMessage () {
	return incomingMessages.pop();
}

bool c_network_api::HasMessages () {
	return !incomingMessages.empty();
};