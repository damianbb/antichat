#include "c_network_api.hpp"
#include "../external/log.hpp"

c_network_api::c_network_api (unsigned short local_port) :
        socket(io_service, udp::endpoint(udp::v4(), local_port)),
        service_thread(&c_network_api::run_service, this),
        handle_remote_error {} {

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

void c_network_api::handle_receive (const boost::system::error_code &error, std::size_t bytes_transferred) {
    if (!error && bytes_transferred < 4096) {
        try {
            client_message message;
            message.msg = std::string(recv_buffer.data(), recv_buffer.data() + bytes_transferred);
            message.remote_endpoint = remote_endpoint;

            if (!message.msg.empty())
                incoming_messages.push(message);

            statistics.register_received_message(bytes_transferred);

            Log::Info("recieved msg from ", remote_endpoint.address());
            Log::Info("     message: ", message.msg);
            Log::Info("     length of msg: ", bytes_transferred);
        } catch (const std::exception &ex) {
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

void c_network_api::send (const std::string &message, const udp::endpoint &target_endpoint) {
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
}

client_message c_network_api::pop_message () {
    return incoming_messages.pop();
}

bool c_network_api::has_messages () {
    return !incoming_messages.empty();
}
