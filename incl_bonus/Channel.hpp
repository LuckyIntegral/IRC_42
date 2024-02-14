

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "General.hpp"

class Server;

# include <set>

class Channel {
private:
	std::string		_name;
	std::string		_password;
	std::string		_topic;
	std::set<int>	_clients;
	std::set<int>	_operators;
	std::set<int>	_invited;
	size_t			_userLimit;
	bool			_inviteOnly;
	bool			_isTopicRestricted;
	Server*			_server;

public:
	Channel();
	Channel( std::string name, std::string password, const int &fd, Server *serv );
	~Channel();

	std::string					getName( void ) const;
	void						setName( const std::string name );
	std::string					getPassword( void ) const;
	void						setPassword( const std::string password );
	std::string					getTopic( void ) const;
	void						setTopic( const std::string topic );
	bool						isInviteOnly( void ) const;
	void						setInviteOnly( const bool &inviteOnly );
	size_t						getUserLimit( void ) const;
	void						setUserLimit( const size_t &userLimit );
	bool						isTopicRestricted( void ) const;
	void						setTopicRestricted( const bool &isTopicRestricted );

	int							addClient( const int &fd , const std::string &password);
	int							removeClient( const int &fd );
	bool						isClient( const int &fd ) const;

	int 						addOperator( const int &fd );
	int 						removeOperator( const int &fd );
	bool						isOperator( const int &fd ) const;

	int 						addInvite( const int &fd );
	int 						removeInvite( const int &fd );
	bool						isInvited( const int &fd ) const;
	void						postMessageInChannel( const std::string &message, const int& fd );
	void						brodcastResponse( const std::string &response );
};


#endif // CHANNEL_HPP
