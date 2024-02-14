#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/poll.h>
# include <fcntl.h>
# include <cstring>
# include <sstream>

# include "General.hpp"

class Client;
class Channel;

class Server
{
	private:
		int			addClient( void );
		int			pollinEvent(const int fd, std::vector<pollfd> &fds);
		int			getClientMessage(int fd, std::string &msg);
		bool		existByUsername(const std::string& username);

		ushort								_port;
		int									_socket;
		std::string							_password;
		struct sockaddr_in					_addr;
		std::vector<pollfd>					_fds;
		std::map<std::string, Channel>		_channels;
		std::map<int, Client>				_clients;
	public:
		Server(ushort port, const std::string& password);
		~Server();

		void		run();
		void		flushResponse(int fd);
		void		prepareResponse(int fd, const std::string& response);
		void		executeCommands( const int &fd, const std::string &line );

		Channel*	addChannel(const std::string& channelName, const std::string& password, const int& fd);
		void		removeChannel(const std::string& channelName);
		Channel*	getChannel(const std::string& channelName);

		std::map<int, Client>&					getClients();
		Client&									getClient(int fd);
		int										getClientIdByNickname(const std::string& nickname);
		std::string								getPassword() const;


		//helping functions
		void									deleteClient(const int fd);
		bool									existByFd(const int fd);
		bool									existByNickname(const std::string& nickname);
		std::string								validateInput(const std::string& input);
		std::string								getAvailableUsername(const std::string& username);
		std::string								getAvailableNickname(const std::string& nickname);

		class SocketCreationException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class SocketBindException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class SocketListenException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class PollException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class AcceptException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class SendException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};

#endif
