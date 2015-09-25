#include <iostream>
#include "c_UDPasync.hpp"
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;
c_UDPasync network_api("fc44:f31b:cae7:5b26:ee5c:f8f3:d2ab:69e7", 12321, 55555);

void __recieve () {
	while (true) {

        if (network_api.HasMessages()) {
            cout << network_api.PopMessage() << endl << "#> ";
            cout.flush();
		}

        this_thread::sleep_for(chrono::milliseconds(100));
	}
}

void __send () {
	string msg, reply;
	while (true) {
        cout << "#> ";
        getline(cin, msg);
        network_api.Send(msg);
	}
}

int main (int argc, char **argv) {
	thread odbjur(__recieve);
	thread wyslij(__send);

	odbjur.join();
	wyslij.join();
	return 0;
}
