#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# define SERVER_NAME "ft_irc"

class Response
{
	private:
		Response();
	public:
		~Response();
		// Connection
		// Successfull connection, takes nickname as argument (const std::string& nickname)
		static const std::string	OKconnectionSuccess(const std::string& nickname, const std::string& username);
		// Failed connection, invalid password, takes nickname as argument (const std::string& nickname)
		static const std::string	ERRconnectionInvalidPassword(const std::string& nickname);

		// Join
		// Successful join, takes nickname and channel as arguments (const std::string& nickname, const std::string& channel)
		static const std::string	OKjoinSuccess(const std::string& nickname, const std::string& username, const std::string& channel);
		// Failed join, takes nickname and channel as arguments (const std::string& nickname, const std::string& channel)
		static const std::string	ERRjoinFailed(const std::string& nickname, const std::string& channel, int error);

		// Nick
		// Successful set of nickname, takes old and new nickname as arguments (const std::string& oldNickname, const std::string& newNickname)
		static const std::string	OKnickSetupSuccess(const std::string& oldNickname, const std::string& newNickname, const std::string& username);
		// Failed set of nickname, takes old and new nickname as arguments (const std::string& oldNickname, const std::string& newNickname)
		static const std::string	ERRnickSetupFailed(const std::string& oldNickname, const std::string& newNickname);

		// Leave
		// Successful leave, takes nickname and channel as arguments (const std::string& nickname, const std::string& channel)
		static const std::string	OKleaveSuccess(const std::string& nickname, const std::string& username, const std::string& channel);
		// Failed leave, takes nickname and channel as arguments (const std::string& nickname, const std::string& channel)
		static const std::string	ERRleaveFailed(const std::string& nickname, const std::string& channel);

		// Messages
		// Successful message, takes nickname, channel and message as arguments (const std::string& nickname, const std::string& message)
		static const std::string	OKmessageSuccess(const std::string& nickname, const std::string& username, const std::string& channel, const std::string& message);
		// Successful private message, takes nicknames (sender, receiver) and message as arguments (const std::string& sender, const std::string& receiver  ,const std::string& message)
		static const std::string	OKprivateMessageSuccess(const std::string& sender, const std::string& senderUsername, const std::string& receiver, const std::string& message);

		// Invite
		// Successful invite, takes nickname and channel as arguments (const std::string& nickname, const std::string& channel)
		static const std::string	OKinviteSuccess(const std::string& nickname, const std::string& username, const std::string& invitedUsername, const std::string& channel);

		// Kick
		// Successful kick, takes nickname, username, kickedUsername, channel and reason as arguments (const std::string& nickname, const std::string& username, const std::string& kickedUsername, const std::string& channel, const std::string& reason)
		static const std::string	OKkickSuccess(const std::string& nickname, const std::string& username, const std::string& kickedUsername, const std::string& channel, const std::string& reason);

		// Set / view channel topic
		// Successful set channel topic, takes nickname and channel as arguments (const std::string& nickname, const std::string& channel)
		static const std::string	OKsetChannelTopicSuccess(const std::string& nickname, const std::string& channel, const std::string& topic);
		// Failed set channel topic, takes nickname and channel as arguments (const std::string& nickname, const std::string& channel)
		static const std::string	ERRsetChannelTopicFailed(const std::string& nickname, const std::string& channel);

		// Quit
		// Successful quit, takes nickname and reason as arguments (const std::string& nickname, const std::string& reason)
		static const std::string	OKquitSuccess(const std::string& nickname, const std::string& username, const std::string& reason);

		// Beautiful error messages
		static const std::string	ERRmsgToChannel(const std::string& nickname, const std::string& command, const std::string& message);
		static const std::string	ERRmsgToUser(const std::string& nickname, const std::string& command, const std::string& message);
		// Unkown command
		// Failed unknown command, takes nickname and command as arguments (const std::string& nickname, const std::string& command)
		static const std::string	ERRunknownCommand(const std::string& nickname, const std::string& command);
		// Beatiful ok message
		static const std::string	OKmsgToChannel(const std::string& nickname, const std::string& username, const std::string& command, const std::string& channel, const std::string& message);

};

#endif
