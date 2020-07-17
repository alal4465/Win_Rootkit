#include "Server.h"
Server::WrapWSA::WrapWSA() {
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		throw std::exception("Error on starting sockets");
	}

}
void Server::WrapWSA::cleanup() {
	std::cout << "WSACleanup called\n";
	WSACleanup();
}
int Server::WrapWSA::getError() {
	return WSAGetLastError();
}

bool Server::RootkitServer::init_listen_bind() {
	sockaddr_in m_adresse;
	m_adresse.sin_addr.s_addr = INADDR_ANY;
	m_adresse.sin_family = AF_INET;
	m_adresse.sin_port = htons(this->m_port);

	int status = bind(this->m_socket, (sockaddr*)&m_adresse, sizeof(m_adresse));

	if (status != 0)
		return false;

	status = listen(this->m_socket, 1);

	if (status != 0)
		return false;

	return true;

}

Server::RootkitServer::RootkitServer() {
	this->m_wsa=Server::WrapWSA();

	this->m_port = Server::SERVER_PORT;

	this->m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	if (this->m_socket == INVALID_SOCKET)
		throw std::exception("Couldn't open socket");


	bool status = this->init_listen_bind();


	if (!status)
		throw std::exception("couldn't listen/bind to port");


}

Server::RootkitServer::~RootkitServer() {

	std::cout << "closing socket.\n";

	closesocket(this->m_socket);
	closesocket(this->m_client_socket);
	this->m_wsa.cleanup();
}

bool Server::RootkitServer::Accept() {

	this->m_client_socket = INVALID_SOCKET;

	// Accept a client socket
	this->m_client_socket = accept(this->m_socket, NULL, NULL);

	if (this->m_client_socket == INVALID_SOCKET)
		return false;

	return true;
}

bool Server::RootkitServer::ReceiveText(std::string& text) {

	char recived_buffer[MAX_PACKET_SIZE]{0};

	int bytes_recived = recv(m_client_socket, recived_buffer, MAX_PACKET_SIZE, 0);

	if (bytes_recived == SOCKET_ERROR) {
		std::cout << "SOCKET ERROR! errorcode:";
		std::cout << this->m_wsa.getError() << "\n";
		return false;
	}

	text += recived_buffer;

	return text.size();
}


bool Server::RootkitServer::SendText(std::string& text) {

	int bytes_recived = send(m_client_socket, text.c_str(), text.size(), 0);

	if (bytes_recived == SOCKET_ERROR) {
		std::cout << "SOCKET ERROR! errorcode:";
		std::cout << this->m_wsa.getError() << "\n";
		return false;
	}

	return true;
}

