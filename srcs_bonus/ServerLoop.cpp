#include "../incl/Server.hpp"

bool	g_running = true;

void	Server::run()
{
	if (bind(this->_socket, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1)
		throw SocketBindException();
	//TODO think about number of clients
	//listen function limits the number of pending connections
	if (listen(this->_socket, 10) == -1)
		throw SocketListenException();
	while (g_running)
	{
		std::vector<pollfd>	temp;

		if (poll(this->_fds.data(), this->_fds.size(), -1) == -1) {
			if (this->_fds.size() != 1)
				throw PollException();
		}

		std::vector<pollfd>::iterator it = this->_fds.begin();
		while (it != this->_fds.end())
		{
			if (it->revents & POLLIN)
			{
				this->pollinEvent(it->fd, temp);
				break ;
			}
			else if (it->revents & POLLOUT)
			{
				this->flushResponse(it->fd);
			}
			++it;
		}

		this->_fds.insert(this->_fds.end(), temp.begin(), temp.end());
	}
	for (std::map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		// send OK quit to everuone
		Client	&client = it->second;
		this->prepareResponse(it->first, Response::OKquitSuccess(client.getNickname(), client.getUsername(), "Server closed"));
		this->flushResponse(it->first);
	}
}

int	Server::pollinEvent(const int fd, std::vector<pollfd> &fds)
{
	if (fd == this->_socket) // if socket fd is triggered, then new client tries to connect
	{
		int	newClientFd = this->addClient();
		if (newClientFd == -1)
			return (ERROR);
		else
		{
			pollfd	newClient;
			newClient.fd = newClientFd;
			newClient.events = POLLIN | POLLOUT;
			newClient.revents = 0;
			fds.push_back(newClient);
		}
	}
	else // if client fd is triggered, then client sent a message
	{
		std::string message;
		if (this->getClientMessage(fd, message) == ERROR)
			return (ERROR);
		Client &client = this->getClient(fd);
		client.setInputBuffer(client.getInputBuffer() + message);
		if (client.getInputBuffer().find("\n") != std::string::npos)
		{
			std::cout << "[Message to parse]: " << client.getInputBuffer() << std::endl;
			message = client.getInputBuffer();
			client.setInputBuffer("");
			this->executeCommands(fd, message);
		}
	}
	return (SUCCESS);
}

int	Server::addClient()
{
	sockaddr_in clientAddr;
	socklen_t	clientAddrLen = sizeof(clientAddr);

	int fd = accept(this->_socket, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (fd == -1)
		return (fd);

	this->_clients.insert(std::pair<int, Client>(fd, Client(fd, this)));

	return (fd);
}
