#include "General.hpp"

static std::vector<std::string>	parseLines( const std::string &str, std::string sep ) {
	std::vector<std::string>	result;
	std::string					line;
	size_t						pos = 0;
	size_t						prev = 0;

	if (str.empty())
		throw int(42);

	while ((pos = str.find(sep, prev)) != std::string::npos) {
		line = str.substr(prev, pos - prev);
		if (!line.empty())
			result.push_back(line);
		prev = pos + sep.length();
	}
	line = str.substr(prev);
	if (!line.empty())
		result.push_back(line);
	return (result);
}

// static void	validateNbrArgs( const std::string &line, const int &nbrArgs, bool isVarArgs = false ) {
// 	std::istringstream	iss(line);
// 	std::string			token;
// 	int					nbr = 0;

// 	while (std::getline(iss, token, ' '))
// 		nbr++;
// 	if (isVarArgs)
// 	{
// 		if (nbr < nbrArgs)
// 			throw std::runtime_error("Wrong number of arguments");
// 	}
// 	else
// 	{
// 		if (nbr != nbrArgs)
// 			throw std::runtime_error("Wrong number of arguments");
// 	}
// }

static std::string	getArgByNbr( const std::string &line, const int &nbr ) {
	std::istringstream	iss(line);
	std::string			token;
	int					nbrArgs = 0;

	while (std::getline(iss, token, ' ')) {
		if (nbrArgs == nbr)
			return (token);
		nbrArgs++;
	}
	return ("");
}

static std::string	getOptionalArg( const std::string &line ) {
	if (line.find(":") != std::string::npos)
		return (line.substr(line.find(":") + 1));
	return ("");
}

static bool	authRequired(int index)
{
	// PASS, NICK, USER, CAP, QUIT
	if (index == 0 || index == 1 || index == 3 || index == 12 || index == 8)
	{
		return (false);
	}
	return (true);
}

static void	executeCommand( const int &fd, const std::string &line, Server *server ) {
	int				index = 0;
	std::string		commands[14] = {
		"PASS",
    	"NICK",
    	"PING",
    	"USER",
    	"TOPIC",
    	"PRIVMSG",
    	"JOIN",
    	"KICK",
    	"QUIT",
    	"INVITE",
    	"PART",
		"MODE",
		"CAP",
		"WHOIS"
	};


	for (index = 0; index < 14; index++)
		if (line.find(commands[index]) == 0)
			break;

	Client&	client = server->getClient(fd);
	if (authRequired(index) && !client.isAuthentificated())
	{
		client.handleNotAuthUser();
		return ;
	}
	std::cout << "index: " << index << std::endl;
	switch (index) {
		case (0) :
			client.setPassword(getArgByNbr(line, 1));
			break;
		case (1) : // NICK
			client.setNickname(getArgByNbr(line, 1));
			break;
		case (2) : // Ping
			client.pong();
			break;
		case (3) : // USER
			client.setUsername(getArgByNbr(line, 1));
			break;
		case (4) : // TOPIC
			client.setTopic(getArgByNbr(line, 1), getOptionalArg(line));
			break;
		case (5) : // PRIVMSG
			client.sendPrvMsg(getArgByNbr(line, 1), getOptionalArg(line));
			break;
		case (6) : // JOIN
			client.joinChannel(getArgByNbr(line, 1), getArgByNbr(line, 2));
			break;
		case (7) : // KICK
			client.kickUser(getArgByNbr(line, 1), getArgByNbr(line, 2), getOptionalArg(line));
			break;
		case (8) : // QUIT
			client.quit(getOptionalArg(line));
			break;
		case (9) : // INVITE
			client.inviteUser(getArgByNbr(line, 1), getArgByNbr(line, 2));
			break;
		case (10):   // LEAVE
			client.leaveChannel(getArgByNbr(line, 1));
			break;
		case (11):   // MODE
			client.setMode(getArgByNbr(line, 2), getArgByNbr(line, 1), getArgByNbr(line, 3));
			break;
		case (12):   // CAP
			break;
		case (13):  //WHOIS
			break;
		default:
			client.handleUnknownCommand(getArgByNbr(line, 0));
			break;
		}
}

void	Server::executeCommands( const int &fd, const std::string &line ) {
	std::vector<std::string> cmds;

	if (line.find("\r\n") != line.npos)
		cmds = parseLines(line, "\r\n");
	else
		cmds = parseLines(line, "\n");

	for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); it++) {
			executeCommand(fd, *it, this);
	}
}
