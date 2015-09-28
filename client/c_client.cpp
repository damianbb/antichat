#include "c_client.hpp"

c_client::c_client(const std::string &host, unsigned short server_port, unsigned short local_port) :
		m_user("anon"),
		m_network_api(host, server_port, local_port) {
}

c_client::~c_client() {
}

void c_client::receive() {
	while (true) {
		if (m_network_api.HasMessages()) {
			string msg = m_network_api.PopMessage();

			if(msg.empty()) {
				continue;
			} else if (msg != "msg recieved") {
				m_network_api.Send("msg recieved");
			}
			std::cout << "[the others] "<< msg << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void c_client::send() {
	string msg;
	while (true) {
		std::getline(cin, msg);
		m_network_api.Send(msg);
	}
}

std::thread c_client::start_sending() {
	return std::thread(&c_client::send,this);
}

std::thread c_client::start_receiving() {
	return std::thread(&c_client::receive,this);
}
