#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <map>

# include "General.hpp"

class Client;

class Channel
{
	private:
		std::string							_name;
		std::string							_password;
		std::string							_topic;
		int									_userLimit;
		bool 								_passwordProtected;
		bool 								_inviteOnly;
		bool								_topicOperatorProtected;
		std::map<int, Client>		_clients;
		std::map<int, Client>		_operators;
	public:
		Channel(const std::string& name, const std::string& password, Client& client);
		~Channel();
		// Accessors
		bool								isPasswordProtected() const;
		bool								isInviteOnly() const;
		bool								isTopicOperatorProtected() const;
		const std::string&					getPassword() const;
		const std::string&					getName() const;
		const std::string&					getTopic() const;
		const std::string&					getClientsList();
		int 								getUserLimit() const;
		std::map<int, Client>&				getClients();
		std::map<int, Client>&				getOperators();
		// Setters	
		void								setLimit(int limit);
		void								setTopic(const std::string& topic);
		void								setPassword(const std::string& password);
		void								setPasswordProtected(bool passwordProtected);
		void								setTopicOperatorProtected(bool topicOperatorProtected);
		void								setInviteOnly(bool inviteOnly);
		void								addClient(Client & client);
		void								removeClient(Client & client);
		void								addOperator(Client & client);
		void								removeOperator(Client & client);
};


# endif
