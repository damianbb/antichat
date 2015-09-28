#include "c_UDPasync.hpp"
#include "c_client.hpp"
#include <iostream>

#include <chrono>
#include <mutex>

std::mutex mtx;

using namespace std;
using namespace std::chrono;

int main (int argc, char *argv[]) {

	try {
		std::ios_base::sync_with_stdio(false);

		c_client test_client("fc44:f31b:cae7:5b26:ee5c:f8f3:d2ab:69e7", 12322, 33333);
		std::thread ts(test_client.start_sending());
		std::thread tr(test_client.start_receiving());
		ts.join();
		tr.join();

		return 0;

	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
}
