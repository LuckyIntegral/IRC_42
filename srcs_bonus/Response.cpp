#include "General.hpp"

Response::Response() {}

Response::~Response() {}

static std::string	to_string(int intValue)
{
	std::stringstream ss;
    ss << intValue;
    return ss.str();
}

const std::string	Response::OKconnectionSuccess(const std::string& nickname, const std::string& username)
{
	std::string response = ":" + std::string(SERVER_NAME) + " 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" + username + "@localhost\r\n";
	return response;
}

const std::string	Response::ERRconnectionInvalidPassword(const std::string& nickname)
{
	std::string response = ":" + std::string(SERVER_NAME) + " 464 " + nickname + " :Invalid password\r\n";
	return response;
}

const std::string	Response::OKjoinSuccess(const std::string& nickname, const std::string& username, const std::string& channel)
{
	std::string response = ":" + nickname + "!" + username + "@localhost JOIN :" + channel + "\r\n";
	return response;
}

const std::string	Response::ERRjoinFailed(const std::string& nickname, const std::string& channel, int error)
{
	std::string response = ":" + std::string(SERVER_NAME) + " " + to_string(error) + " " + nickname + " " + channel + " :Cannot join channel (+k)\r\n";
	return response;
}

const std::string	Response::OKnickSetupSuccess(const std::string& oldNickname, const std::string& newNickname, const std::string& username)
{
	std::string response = ":" + oldNickname + "!" + username + "@localhost NICK :" + newNickname + "\r\n";
	return response;
}

const std::string	Response::ERRnickSetupFailed(const std::string& oldNickname, const std::string& newNickname)
{
	std::string response = ":" + oldNickname + "!" + std::string(SERVER_NAME) + " 433 * " + newNickname + " :Nickname is already in use\r\n";
    return response;
}

const std::string	Response::OKleaveSuccess(const std::string& nickname, const std::string& username, const std::string& channel)
{
	std::string response = ":" + nickname + "!" + username + "@localhost PART " + channel  + "\r\n";
	return response;
}

const std::string	Response::ERRleaveFailed(const std::string& nickname, const std::string& channel)
{
	std::string response = ":" + std::string(SERVER_NAME) + " 403 " + nickname + " " + channel + " :No such channel\r\n";
	return response;
}

const std::string	Response::OKmessageSuccess(const std::string& nickname, const std::string& username, const std::string& channel, const std::string& message)
{
	std::string response = ":" + nickname + "!" + username + "@localhost PRIVMSG " + channel + " :" + message + "\r\n";
	return response;
}

// не уверен, что это правильно
const std::string	Response::OKprivateMessageSuccess(const std::string& sender, const std::string& senderUsername, const std::string& receiver, const std::string& message)
{
	std::string response = ":" + sender + "!" + senderUsername + "@localhost PRIVMSG " + receiver + " :" + message + "\r\n";
	return response;
}

const std::string	Response::OKinviteSuccess(const std::string& nickname, const std::string& username, const std::string& invitedUsername, const std::string& channel)
{
	std::string response = ":" + nickname + "!" + username + "@localhost 341 " + nickname + " " + invitedUsername + " " + channel + "\r\n";
	return response;
}


// :vfrants!vfrants@localhost KICK #tst vfrants_ :Kicked by the channel's operator

const std::string	Response::OKkickSuccess(const std::string& nickname, const std::string& username, const std::string& kickedUsername, const std::string& channel, const std::string& reason)
{
	std::string response = ":" + nickname + "!" + username + "@localhost KICK " + channel + " " + kickedUsername + " :" + reason + "\r\n";
	return response;
}

const std::string	Response::OKsetChannelTopicSuccess(const std::string& nickname, const std::string& channel, const std::string& topic)
{
	std::string response = ":" + std::string(SERVER_NAME) + " 332 " + nickname + " " + channel + " :" + topic + "\r\n";
	return response;
}

const std::string	Response::ERRsetChannelTopicFailed(const std::string& nickname, const std::string& channel)
{
	std::string response = ":" + std::string(SERVER_NAME) + " 442 " + nickname + " " + channel + " :Topic set failed\r\n";
	return response;
}

const std::string	Response::ERRunknownCommand(const std::string& nickname, const std::string& command)
{
	std::string response = ":" + std::string(SERVER_NAME) + " 421 " + nickname + " " + command + " :Unknown command\r\n";
	return response;
}

const std::string	Response::ERRmsgToChannel(const std::string& nickname, const std::string& command, const std::string& message)
{
	std::string response = ":" + std::string(SERVER_NAME) + " 482 " + nickname + " " + command + " :" + message + "\r\n";
	return response;
}

const std::string	Response::ERRmsgToUser(const std::string& nickname, const std::string& command, const std::string& message)
{
	std::string response = ":" + std::string(SERVER_NAME) + " 400 " + nickname + " :" + command + " :" + message + "\r\n";
	return response;
}

const std::string	Response::OKquitSuccess(const std::string& nickname, const std::string& username, const std::string& reason)
{
	std::string response = ":" + nickname + "!" + username + "@localhost QUIT :" + reason + "\r\n";
	return response;
}

const std::string	Response::OKmsgToChannel(const std::string& nickname, const std::string& username, const std::string& command, const std::string& channel, const std::string& message)
{
	std::string response = ":" + nickname + "!" + username + "@localhost " + command + " " + channel + " :" + message + "\r\n";
	return response;
}
