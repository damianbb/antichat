#include <iostream>
#include "c_UDPasync.hpp"
#include <chrono>
#include <thread>
#include <string>

using namespace std;
using namespace std::chrono;
c_UDPasync network_api("192.168.0.105", 12321, 55555);

void _receive () {
  while (true) {

    if (network_api.HasMessages()) {
      cout << network_api.PopMessage() << endl << "#> ";
      cout.flush();
    }

    this_thread::sleep_for(chrono::milliseconds(100));
  }
}

void _send () {
  string msg;
  while (true) {
    cout << "#> ";
    getline(cin, msg);
    network_api.Send(msg);
  }
}

int main (int argc, char **argv) {
  thread t_receive(_receive);
  thread t_send(_send);

  t_receive.join();
  t_send.join();
  return 0;
}
