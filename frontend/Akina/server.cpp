#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <thread>

#include <bits/stdc++.h>

#include "jsoncpp/json/json.h"
#include "Command.hpp"
#include "UserMananger.hpp"

namespace server {
	using std::string, std::cerr, std::endl, std::stringstream, std::istream, std::ostream;

	const int buffer_size = 1048576;
	socklen_t sock_length = sizeof(sockaddr_in);

	string server_ip = "127.0.0.1";
	int server_port = 3012;

	int server_id, client_id;
	struct sockaddr_in server_addr, client_addr;
	bool is_started = false;

	Json::Reader reader;
	Json::FastWriter writer;

	char buffer[buffer_size + 1];

	void stderr_Failed(string s = "") {
		cerr << "\033[1;31mFailed\033[0m " << s << endl;
	}

	void stderr_OK(string s = "") {
		cerr << "\033[1;32mOK\033[0m " << s << endl;
	}

	void JSON_Error(string err) {
		Json::Value root;
		root["error"] = err;
		root["result"] = "";

		string ret = writer.write(root);
		strcpy(buffer, ret.c_str());

		stderr_Failed(" " + err);
	}

	void JSON_OK(string res) {
		Json::Value root;
		root["error"] = stderr_OK;
		root["result"] = res;

		string ret = writer.write(root);
		strcpy(buffer, ret.c_str());
	}

	int shell(char buf[], int len) {
		static stringstream is, os;
		static UserManager user_manager;
		static TrainManager train_manager;
		
		is.str(buf);
		os.str("");

		bool no_exit = 1;
		
		Main_Command(*((istream*)(&is)), *((ostream*)(&os)), user_manager, train_manager, no_exit);

		if (!no_exit) {
			return -1;
		}
		else {
			return 0;
		}
	}


	void load_config() {
		cerr << "loading config...";
		FILE* config = fopen("config.ini", "r");
		if (!config) {
			stderr_Failed("file error, using default config.");
			return;
		}
		fread(buffer, 1, buffer_size, config);
		fclose(config);

		Json::Value root;
		
		if (!reader.parse(buffer, buffer + strlen(buffer), root)) {
			stderr_Failed("parse failed, using default config.");
			return;
		}
		try {
			string ip = root["server_ip"].asString();
			int port = root["server_port"].asInt();
			server_ip = ip;
			server_port = port;
		}
		catch (...) {
			stderr_Failed("parameters unrecognized, using default config.");
			return;
		}
	
		stderr_OK();
	}

	bool server_start() {
		cerr << "starting server...";
		if (is_started) {
			close(server_id);
		}
		server_id = socket(AF_INET, SOCK_STREAM, 0);
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(server_port);
		server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
		if (bind(server_id, (struct sockaddr* ) &server_addr, sizeof(server_addr)) == -1) {
			stderr_Failed("server address unavailable");
			return false;
		}
		stderr_OK();
		return true;
	}

	void listening() {
		while (true) {
			cerr << "started listening " << server_ip << ":" << server_port << endl;
			client_id = accept(server_id, (struct sockaddr*)&client_addr, &sock_length);
			if (client_id < 0) {
				stderr_Failed("listen failed");
				return;
			}
			stderr_OK((string)"connection established : " + inet_ntoa(client_addr.sin_addr));
			int len = 0;
			try {
				int len = recv(client_id, buffer, buffer_size, 0);
				if (len < 0) {
					throw;
				}
				buffer[len] = '\0';
				stderr_OK("successfully received " + std::to_string(len) + " bytes");	
			}
			catch (...) {
				stderr_Failed("connection is broken");
				close(client_id);
				return;
			}

			int ret = shell(buffer, len);

			send(client_id, buffer, strlen(buffer), 0);
			close(client_id);

			if (ret == -1) {
				stderr_OK("bye!");
				break;
			}
			else if (ret == 0) {
				stderr_OK("operation ok");
			}
		}
	}

	void server_end() {
		cerr << "closing server...";
		if (is_started) {
			close(server_id);
		}
		stderr_OK();
	}

	void init() {
		load_config();
		server_start();
	}

	void fin() {
		server_end();
	}

}

int main() {
	server::init();
	server::listening();
	server::fin();
}
