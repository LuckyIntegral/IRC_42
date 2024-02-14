#include "General.hpp"

Channel::Channel() {}
Channel::Channel( std::string name, std::string password, const int& fd, Server *serv )
{
	std::cout << "Channel created " + name << std::endl;
	this->_name = name;
	this->_password = password;
	this->_clients.insert(fd);
	this->_operators.insert(fd);
	this->_inviteOnly = false;
	this->_isTopicRestricted = false;
	this->_userLimit = 4294967295;
	this->_server = serv;
}
Channel::~Channel() {}

std::string	Channel::getName( void ) const
{
	return (this->_name);
}

void	Channel::setName( const std::string name )
{
	this->_name = name;
}

std::string	Channel::getPassword( void ) const
{
	return (this->_password);
}

void	Channel::setPassword( const std::string password )
{
	this->_password = password;
}

std::string	Channel::getTopic( void ) const
{
	return (this->_topic);
}

void	Channel::setTopic( const std::string topic )
{
	this->_topic = topic;
}

bool	Channel::isInviteOnly( void ) const
{
	return (this->_inviteOnly);
}

void	Channel::setInviteOnly( const bool &inviteOnly )
{
	this->_inviteOnly = inviteOnly;
}

size_t	Channel::getUserLimit( void ) const
{
	return (this->_userLimit);

}
void	Channel::setUserLimit( const size_t &userLimit )
{
	this->_userLimit = userLimit;
}

bool	Channel::isTopicRestricted( void ) const
{
	return (this->_isTopicRestricted);

}

void	Channel::setTopicRestricted( const bool &isTopicRestricted )
{
	this->_isTopicRestricted = isTopicRestricted;
}


int		Channel::addClient( const int &fd, const std::string &password )
{
	// if channel is full
	if (this->_clients.size() >= this->_userLimit)
		return ERR_CHANNELISFULL;
	// if channel is invite only
	if (this->_inviteOnly)
	{
		if (this->_invited.count(fd) == 0)
			return ERR_INVITEONLYCHAN;
	}
	// if user not in channel and is invited or password is correct
	if (this->_clients.count(fd) == 0 && (this->isInvited(fd) || this->_password == password))
	{
		this->_clients.insert(fd);
		this->removeInvite(fd);
		return SUCCESS;
	}
	return ERR_BADCHANNELKEY;
}

int		Channel::removeClient( const int &fd )
{
	if (this->_clients.count(fd) == 1)
	{
		this->_clients.erase(fd);
		this->removeOperator(fd);
		return SUCCESS;
	}
	return ERROR;
}

bool	Channel::isClient( const int &fd ) const
{
	return (this->_clients.count(fd) == 1);
}

int		Channel::addOperator( const int &fd )
{
	if (this->_clients.count(fd) == 1 && this->_operators.count(fd) == 0)
	{
		this->_operators.insert(fd);
		return SUCCESS;
	}
	return ERROR;
}

int		Channel::removeOperator( const int &fd )
{
	if (this->_operators.count(fd) == 1)
	{
		this->_operators.erase(fd);
		return SUCCESS;
	}
	return ERROR;
}

bool	Channel::isOperator( const int &fd ) const
{
	return (this->_operators.count(fd) == 1);
}

int 	Channel::addInvite( const int &fd )
{
	if (this->_invited.count(fd) == 0)
	{
		this->_invited.insert(fd);
		return SUCCESS;
	}
	return ERROR;
}

int 	Channel::removeInvite( const int &fd )
{
	if (this->_invited.count(fd) == 1)
	{
		this->_invited.erase(fd);
		return SUCCESS;
	}
	return ERROR;
}

bool	Channel::isInvited( const int &fd ) const
{
	return (this->_invited.count(fd) == 1);
}

void	Channel::postMessageInChannel( const std::string &message, const int &fd )
{
	for (std::set<int>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (*it != fd)
			this->_server->prepareResponse(*it, message);
	}
}

void	Channel::brodcastResponse( const std::string &response )
{
	for (std::set<int>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		this->_server->prepareResponse(*it, response);
	}
}
