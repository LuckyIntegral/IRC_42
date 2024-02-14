#ifndef GENERAL_HPP
# define GENERAL_HPP

# define ERROR 1
# define SUCCESS 0
# define ERR_CHANNELISFULL 471
# define ERR_BADCHANNELKEY 475
# define ERR_INVITEONLYCHAN 473

#include <string>
#include <csignal>
#include <unistd.h>

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Response.hpp"

class Server;
class Channel;
class Client;

extern bool g_running;

bool    isValidLimit(const std::string& limit);

#endif
