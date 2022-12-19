#include "spacex.hpp"

namespace {

#ifdef LEAK
	void
	ft_handler_leak_() {
		system("leaks spacex");
	}
#endif

#ifdef SOCKET_DEBUG
	inline void
	port_info_print_(port_info_map const& port_info) {
		for (port_info_map::const_iterator it = port_info.begin(); it != port_info.end(); ++it) {
			std::cout << "------------------------------------" << std::endl;
			std::cout << "port: " << it->first << std::endl;
			std::cout << "listen_sd: " << it->second.listen_sd << std::endl;
			std::cout << "my_port: " << it->second.my_port << std::endl;
			std::cout << "default_server info print: " << std::endl;
			it->second.my_port_default_server.print();
			for (server_map_p::const_iterator it2 = it->second.my_port_map.begin(); it2 != it->second.my_port_map.end(); ++it2) {
				if (it2->second.default_server_flag == Kother_server) {
					it2->second.print();
				}
			}
		}
	}
#endif

	inline total_port_server_map_p
	config_file_open_(int argc, char const* argv[], std::string const& cur_path) {
		std::fstream file;
		switch (argc) {
		case 1:
			file.open("./config/default.conf", std::ios_base::in);
			break;
		case 2:
			file.open(argv[1], std::ios_base::in);
			break;
		default:
			error_exit_msg("usage: ./spacex [config_file]");
		}
		if (file.is_open() == false) {
			error_exit_msg_perror("file open error");
		}
		std::stringstream ss;
		ss << file.rdbuf();
		if (ss.fail()) {
			file.close();
			error_exit_msg_perror("file read error");
		}
		file.close();
#ifdef MAIN_DEBUG
		std::cout << ss.str() << std::endl;
#endif
		total_port_server_map_p temp_config;
		if (spx_config_syntax_checker(ss.str(), temp_config, cur_path) != spx_ok) {
			error_exit_msg("config syntax error");
		}
		return temp_config;
	}

	inline void
	get_current_path_(std::string& cur_path) {
		char buf[8192];
		if (getcwd(buf, sizeof(buf)) == NULL) {
			error_exit_msg_perror("getcwd error");
		}
		cur_path = buf;
	}

} // namespace

int
main(int argc, char const* argv[]) {
#ifdef LEAK
	atexit(ft_handler_leak_);
#endif
	if (argc <= 2) {
		std::string cur_dir;
		get_current_path_(cur_dir);
		spx_log_(cur_dir);

		total_port_server_map_p config_info = config_file_open_(argc, argv, cur_dir);
		spx_log_("config file open success");

		main_info_t spx;
		socket_init_and_build_port_info(config_info, spx.port_info);
		spx_log_("socket per port_info success");
#ifdef SOCKET_DEBUG
		port_info_print_(spx.port_info);
#endif
		// TODO:: add kqueue process here
		// while (1) {
		// }

	} else {
		error_exit_msg("usage: ./spacex [config_file]");
	}
	return spx_ok;
}
