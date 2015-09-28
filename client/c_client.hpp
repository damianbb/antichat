#ifndef C_CLIENT_H
#define C_CLIENT_H
#include <thread>
#include <list>
#include <chrono>
#include "c_user.hpp"
#include "c_UDPasync.hpp"

class c_client {
  public:
	c_client(const std::string &host, unsigned short server_port, unsigned short local_port = 0);
	~c_client();
	std::thread start_sending();
	std::thread start_receiving();

	c_UDPasync m_network_api;
  private:
	void send();
	void receive();

	c_user m_user;
};

#endif // C_CLIENT_H

