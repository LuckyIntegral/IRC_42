#include "General.hpp"
#include <cctype>

Client::Client() {}

Client::Client(int fd, Server *serv)
{
	this->_fd = fd;
	this->_isAuthentificated = false;
	this->_isPasswordSet = false;
	this->_isUsernameSet = false;
	this->_isNicknameSet = false;
	this->_server = serv;
	this->_outputBuffer = "";
	this->_inputBuffer = "";
}

Client::~Client() {}

//////////////////////////////////////////////////////////////////////////////////////////

// Accessors

int Client::getFd() const
{
	return this->_fd;
}

std::string Client::getUsername() const
{
	return this->_username;
}

std::string Client::getNickname() const
{
	return this->_nickname;
}

std::string Client::getPassword() const
{
	return this->_password;
}

std::string Client::getInputBuffer() const
{
	return this->_inputBuffer;
}

std::string Client::getOutputBuffer() const
{
	return this->_outputBuffer;
}

bool Client::isAuthentificated() const
{
	return this->_isAuthentificated;
}

void 		Client::setUsername(std::string username)
{
	// if username has a non valid character, replace it with '_'
	// valid characters are: [a-zA-Z0-9_]
	username = this->_server->validateInput(username);

	this->_username = this->_server->getAvailableUsername(username);
	this->_isUsernameSet = true;
	this->checkAndSetAuthentificated();
}

void Client::setNickname(std::string nickname)
{
	// if username has a non valid character, replace it with '_'
	// valid characters are: [a-zA-Z0-9_]
	nickname = this->_server->validateInput(nickname);

	// if nickname not set, set it
	if (!this->_isNicknameSet)
	{
		this->_nickname = this->_server->getAvailableNickname(nickname);
		this->_isNicknameSet = true;
		this->checkAndSetAuthentificated();
		return ;
	}
	// otherwise, change it
	std::string newNickname = std::string(nickname.begin(), nickname.end());
	if (this->_server->existByNickname(newNickname))
		this->_server->prepareResponse(this->_fd, Response::ERRnickSetupFailed(this->_nickname, newNickname));
	else
	{
		for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
		{
			std::string response = Response::OKmessageSuccess(this->_nickname, this->_username, (*it)->getName(), "has changed his nickname to " + newNickname);
			(*it)->brodcastResponse(response);
		}
		for (std::map<int, Client>::iterator it = this->_server->getClients().begin(); it != this->_server->getClients().end(); it++)
		{
			this->_server->prepareResponse(it->second.getFd(), Response::OKnickSetupSuccess(this->_nickname, newNickname, this->_username));
		}
		this->_nickname = newNickname;
	}
}

void Client::setPassword(const std::string& password)
{
	this->_password = password;
	this->_isPasswordSet = true;
	this->checkAndSetAuthentificated();
}

void Client::setInputBuffer(std::string buffer)
{
	this->_inputBuffer = buffer;
}

void Client::setOutputBuffer(std::string buffer)
{
	this->_outputBuffer = buffer;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Command Handler

void		Client::checkAndSetAuthentificated()
{
	if (!(this->_isPasswordSet && this->_isUsernameSet && this->_isNicknameSet))
		return ;
	if (this->_isAuthentificated)
		return ;
	if (this->_password == this->_server->getPassword())
	{
		this->_isAuthentificated = true;
		this->_server->prepareResponse(this->_fd, Response::OKconnectionSuccess(this->_nickname, this->_username));
	}
	else
	{
		this->_server->prepareResponse(this->_fd, Response::ERRconnectionInvalidPassword(this->_nickname));
		this->_isAuthentificated = false;
	}
}

// methods

void		Client::pong( void )
{
	this->_server->prepareResponse(this->_fd, "PONG :localhost\r\n");
}

void		Client::joinChannel(const std::string& channelName, const std::string& password)
{
	// if username has a non valid character, replace it with '_'
	// valid characters are: [a-zA-Z0-9_], the rest is replaced with '_'
	for (size_t i = 0; i < channelName.size(); i++)
	{
		if (i == 0)
			continue ;
		if (!(std::isalnum(channelName[i]) || channelName[i] == '_'))
		{
			this->_server->prepareResponse(this->_fd, Response::ERRjoinFailed(this->_nickname, channelName, 403));
			return ;
		}
	}

	// std::string	channelName = this->_server->validateInput(channelName);
	// channelName[0] = '#';
	// check if user is already in this channel
	if (this->_channels.size() > 0)
	{
		for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
		{
			if ((*it)->getName() == channelName)
			{
				this->_channels.erase(it);
				this->_channels.push_back(this->_server->getChannel(channelName));
				return ; // TODO: maybe we need to send a response to the client
			}
		}
	}

	Channel *channel = this->_server->getChannel(channelName);
	// if channel does not exist, create it
	if (channel == NULL)
	{
		this->_channels.push_back(this->_server->addChannel(channelName, password, this->_fd));
		this->_server->prepareResponse(this->_fd, Response::OKjoinSuccess(this->_nickname, this->_username, channelName));
		std::string response = Response::OKmessageSuccess(this->_nickname, this->_username, channelName, "has joined the channel");
		this->_channels.back()->brodcastResponse(response);
		return ;
	}
	// if channel exists
	int code = channel->addClient(this->_fd, password);
	if (code != SUCCESS)
	{
		this->_server->prepareResponse(this->_fd, Response::ERRjoinFailed(this->_nickname, channelName, code));
		return ;
	}
	this->_channels.push_back(channel);
	this->_server->prepareResponse(this->_fd, Response::OKjoinSuccess(this->_nickname, this->_username, channelName));
	std::string response = Response::OKmessageSuccess(this->_nickname, this->_username, channel->getName(), "has joined the channel");
	this->_channels.back()->brodcastResponse(response);
	if (channel->getTopic() != "")
		this->_server->prepareResponse(this->_fd, Response::OKsetChannelTopicSuccess(this->_nickname, channelName, channel->getTopic()));
}

void		Client::leaveChannel(const std::string& channelName)
{
	// check if user is in this channel
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		// if user is in this channel remove him from it
		if ((*it)->getName() == channelName)
		{
			(*it)->removeClient(this->_fd);
			(*it)->removeOperator(this->_fd);
			(*it)->removeInvite(this->_fd);
			this->_server->prepareResponse(this->_fd, Response::OKleaveSuccess(this->_nickname, this->_username, channelName));
			std::string response = Response::OKmessageSuccess(this->_nickname, this->_username, (*it)->getName(), "has left the channel");
			(*it)->brodcastResponse(response);
			std::cout << '\'' << this->_nickname << '\'' << " has left the channel" << std::endl;
			this->_channels.erase(it);
			return ;
		}
	}
	this->_server->prepareResponse(this->_fd, Response::ERRleaveFailed(this->_nickname, channelName));
}

void	Client::setTopic(const std::string& channelName, const std::string& topic) {
	Channel *channel = this->_server->getChannel(channelName);

	if (channel == NULL)
	{
		this->_server->prepareResponse(this->_fd, Response::ERRsetChannelTopicFailed(this->_nickname, channelName));
		return ;
	}
	if (!channel->isClient(this->_fd))
	{
		this->_server->prepareResponse(this->_fd, Response::ERRsetChannelTopicFailed(this->_nickname, channelName));
		return ;
	}
	if (channel->isTopicRestricted() && !channel->isOperator(this->_fd))
	{
		this->_server->prepareResponse(this->_fd, Response::ERRsetChannelTopicFailed(this->_nickname, channelName));
		return ;
	}
	channel->setTopic(topic);
	channel->brodcastResponse(Response::OKsetChannelTopicSuccess(this->_nickname, channelName, topic));
}

void	Client::sendPrvMsg(const std::string& nickname, const std::string& message)
{
	if (nickname == "")
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "PRIVMSG", "No recipient given"));
		return ;
	}
	if (nickname == this->_nickname)
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "PRIVMSG", "Cannot send message to yourself"));
		return ;
	}
	if (nickname[0] == '#')
	{
		Channel *channel = this->_server->getChannel(nickname);

		if (channel != NULL)
		{
			if (!channel->isClient(this->_fd))
				this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "PRIVMSG", "You are not in the channel"));
			else
			{
				std::string response = Response::OKmessageSuccess(this->_nickname, this->_username, channel->getName(), message);
				channel->postMessageInChannel(response, this->_fd);
			}
		}
		else
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "PRIVMSG", "Channel does not exist"));
		}
		return ;
	}

	int fd = this->_server->getClientIdByNickname(nickname);
	if (fd == -1)
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "PRIVMSG", "User does not exist"));
		return ;
	}
	Client& client = this->_server->getClient(fd);
	if (!client.isAuthentificated())
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "PRIVMSG", "User does not exist"));
		return ;
	}
	this->_server->prepareResponse(this->_server->getClientIdByNickname(nickname), Response::OKprivateMessageSuccess(this->_nickname, this->_username, nickname, message));
}

void	Client::kickUser(const std::string& channelName, const std::string& nickname, const std::string& reason)
{
	Channel *channel = this->_server->getChannel(channelName);

	if (channel == NULL || !channel->isClient(this->_fd))
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "KICK", "You are not in the channel"));
		return ;
	}
	if (!channel->isOperator(this->_fd))
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, channelName, "You are not an operator"));
		return ;
	}
	if (!channel->isClient(this->_server->getClientIdByNickname(nickname)))
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, channelName, "User is not in the channel"));
		return ;
	}
	channel->removeClient(this->_server->getClientIdByNickname(nickname));
	Client& kickedClient = this->_server->getClient(this->_server->getClientIdByNickname(nickname));
	kickedClient.leaveChannel(channelName);
	channel->brodcastResponse(Response::OKkickSuccess(this->_nickname, this->_username, nickname, channelName, reason));
}

void	Client::inviteUser(const std::string& nickname, const std::string& channelName)
{
	Channel *channel = this->_server->getChannel(channelName);

	if (channel == NULL || !channel->isClient(this->_fd))
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "INVITE", "You are not in the channel"));
		return ;
	}
	// if (!channel->isOperator(this->_fd))
	// {
	// 	this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, channelName, "You are not an operator"));
	// 	return ;
	// }
	if (channel->isClient(this->_server->getClientIdByNickname(nickname)))
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, channelName, "User is already in the channel"));
		return ;
	}
	if (channel->isInvited(this->_server->getClientIdByNickname(nickname)))
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, channelName, "User is already invited"));
		return ;
	}
	int fd = this->_server->getClientIdByNickname(nickname);
	if (fd == -1)
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, channelName, "User does not exist"));
		return ;
	}
	Client& client = this->_server->getClient(fd);
	if (!client.isAuthentificated())
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, channelName, "User does not exist"));
		return ;
	}
	channel->addInvite(fd);
	this->_server->prepareResponse(this->_server->getClientIdByNickname(nickname), Response::OKinviteSuccess(this->_nickname, this->_username, nickname, channelName));
}

void	Client::quit(const std::string& reason)
{
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		std::string response = Response::OKmessageSuccess(this->_nickname, this->_username, (*it)->getName(), "has quit the server (" + reason + ")");
		(*it)->brodcastResponse(response);
		(*it)->removeClient(this->_fd);
	}
	this->_server->prepareResponse(this->_fd, Response::OKquitSuccess(this->_nickname,this->_username, reason));
	this->_server->flushResponse(this->_fd);
	this->_server->deleteClient(this->_fd);
}

void	Client::handleInviteMode(const std::string& mode, Channel *chan)
{
	if (mode == "+i")
	{
		if (chan->isInviteOnly())
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "Channel is already invite only"));
			return ;
		}
		chan->setInviteOnly(true);
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), "Invite only mode set"));
	}
	if (mode == "-i")
	{
		if (!chan->isInviteOnly())
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "Channel is not invite only"));
			return ;
		}
		chan->setInviteOnly(false);
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), "Invite only mode unset"));
	}
}


void	Client::handleTopicMode(const std::string& mode, Channel *chan)
{
	if (mode == "+t")
	{
		if (chan->isTopicRestricted())
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "Channel topic is already restricted"));
			return ;
		}
		chan->setTopicRestricted(true);
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), "Topic restricted mode set"));
	}
	if (mode == "-t")
	{
		if (!chan->isTopicRestricted())
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "Channel topic is not restricted"));
			return ;
		}
		chan->setTopicRestricted(false);
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), "Topic restricted mode unset"));
	}
}

void	Client::handlePasswordMode(const std::string& mode, Channel *chan, const std::string& password)
{
	if (mode == "+k")
	{
		if (password == "")
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "Cannot set an empty password"));
			return ;
		}
		chan->setPassword(password);
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), "Password set"));
	}
	if (mode == "-k")
	{
		if (chan->getPassword() == "")
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "Channel does not have a password"));
			return ;
		}
		chan->setPassword("");
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), "Password unset"));
	}
}

void	Client::handleOperatorMode(const std::string& mode, Channel *chan, const std::string& nickname)
{
	if (mode == "+o")
	{
		if (chan->isOperator(this->_server->getClientIdByNickname(nickname)))
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "User is already an operator"));
			return ;
		}
		if (!chan->isClient(this->_server->getClientIdByNickname(nickname)))
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "User is not in the channel"));
			return ;
		}
		chan->addOperator(this->_server->getClientIdByNickname(nickname));
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), nickname +  " is now an operator"));
	}
	if (mode == "-o")
	{
		if (!chan->isClient(this->_server->getClientIdByNickname(nickname)))
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "User is not in the channel"));
			return ;
		}
		if (!chan->isOperator(this->_server->getClientIdByNickname(nickname)))
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "User is not an operator"));
			return ;
		}
		chan->removeOperator(this->_server->getClientIdByNickname(nickname));
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), nickname + " is no longer an operator"));
	}
}

void	Client::handleLimitMode(const std::string& mode, Channel *chan, const std::string& limit)
{
	// validate limit
	if (!isValidLimit(limit))
	{
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "Invalid limit"));
		return ;
	}
	if (mode == "+l")
	{
		std::stringstream ss(limit);
		size_t ulimit;
		ss >> ulimit;
		chan->setUserLimit(ulimit);
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), "Limit set to " + limit));
	}
	if (mode == "-l")
	{
		if (chan->getUserLimit() == MAX_CHAN_LIMIT)
		{
			this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, chan->getName(), "Channel does not have a limit"));
			return ;
		}
		chan->setUserLimit(MAX_CHAN_LIMIT);
		chan->brodcastResponse(Response::OKmsgToChannel(this->_nickname, this->_username, "MODE", chan->getName(), "Limit unset"));
	}
}

void	Client::setMode(const std::string& mode, const std::string& channel, const std::string& arg)
{
	Channel *chan = this->_server->getChannel(channel);

	if (channel == "" || channel[0] != '#')
		return ;
	if (chan == NULL || !chan->isClient(this->_fd))
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "MODE", "You are not in the channel"));
	else if (!chan->isOperator(this->_fd))
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToChannel(this->_nickname, channel, "You are not an operator"));
	else if (mode == "+i" || mode == "-i")
		this->handleInviteMode(mode, chan);
	else if (mode == "+t" || mode == "-t")
		this->handleTopicMode(mode, chan);
	else if (mode == "+k" || mode == "-k")
		this->handlePasswordMode(mode, chan, arg);
	else if (mode == "+o" || mode == "-o")
		this->handleOperatorMode(mode, chan, arg);
	else if (mode == "+l" || mode == "-l")
		this->handleLimitMode(mode, chan, arg);
	else
		this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "MODE", "Invalid mode"));
}

void	Client::handleUnknownCommand(const std::string& command)
{
	this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, command, "Unknown command"));
}

void	Client::handleNotAuthUser()
{
	this->_server->prepareResponse(this->_fd, Response::ERRmsgToUser(this->_nickname, "", "You are not authentificated"));
}
