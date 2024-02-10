#include "Server.hpp"

//Constructor && Destructor

Server::Server(ushort port, const std::string& password)
{
	this->_port = port;
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == -1)
		throw SocketCreationException();
	int flags = fcntl(this->_socket, F_SETFL, O_NONBLOCK);
	if (flags == -1)
		throw SocketCreationException();

	int opt = 1;
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw SocketCreationException();
	this->_password = password;

	struct pollfd pfd;
	pfd.fd = this->_socket;
	pfd.events = POLLIN;
	pfd.revents = 0;
	this->_fds.push_back(pfd);
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(this->_port);
	this->_addr.sin_addr.s_addr = INADDR_ANY;
}

Server::~Server()
{
	close(this->_socket);
}
/////////////////////////////////////////////////////////////////////////////////////////////////

//Exceptions

const char*				Server::SendException::what() const throw()
{
	return "Send failure";
}

const char*				Server::PollException::what() const throw()
{
	return "Poll failure";
}

const char*				Server::SocketCreationException::what() const throw()
{
	return "Socket creation failure";
}

const char*				Server::SocketBindException::what() const throw()
{
	return "Socket bind failure";
}

const char*				Server::SocketListenException::what() const throw()
{
	return "Socket listen failure";
}

const char*				Server::AcceptException::what() const throw()
{
	return "Socket accept failure";
}

std::string				Server::getPassword() const
{
	return this->_password;
}

Client&					Server::getClient(int fd)
{
	return this->_clients[fd];
}

Channel*				Server::addChannel(const std::string& channelName, const std::string& password, const int& fd)
{
	this->_channels[channelName] = Channel(channelName, password, fd, this);
	return &this->_channels[channelName];
}

void					Server::removeChannel(const std::string& channelName)
{
	this->_channels.erase(channelName);
}

Channel*				Server::getChannel(const std::string& channelName)
{
	if (this->_channels.find(channelName) == this->_channels.end())
		return NULL;
	return &this->_channels[channelName];
}

// helping methods

bool					Server::existByUsername(const std::string& username)
{
	for (std::map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second.getUsername() == username)
			return true;
	}
	return false;
}

bool					Server::existByNickname(const std::string& nickname)
{
	for (std::map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second.getNickname() == nickname)
			return true;
	}
	return false;
}

std::string				Server::getAvailableUsername(const std::string& username)
{
	std::string resUsername = username;
	while (this->existByUsername(resUsername))
	{
		resUsername += "🤡";
	}
	return resUsername;
}

std::string				Server::getAvailableNickname(const std::string& nickname)
{
	std::string resNickname = nickname;
	while (this->existByNickname(resNickname))
	{
		resNickname += "🤡";
	}
	return resNickname;
}

std::map<int, Client>&	Server::getClients()
{
	return this->_clients;
}