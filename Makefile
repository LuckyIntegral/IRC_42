################################################################################
#####                        GENERAL PROJECT SETTINGS                      #####
################################################################################

CXX			= c++
RM			= rm -rf

CXXFLAGS	= -Wall -Wextra -Werror -MD -MP -g -std=c++98 -pedantic

MAKEFLAGS	= -j$(nproc)

################################################################################
#####                             MANDATORY PART                           #####
################################################################################

NAME		= ircserv
INCLUDES	= -I ./incl
SRCS_DIR	= srcs
OBJS_DIR	= objs

SRCS	= $(wildcard $(SRCS_DIR)/*.cpp)
OBJS	= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))
DEPS	= $(OBJS:.o=.d)

################################################################################
#####                               BONUS PART                             #####
################################################################################

NAME_B		= ircserv_bonus
INCLUDES_B	= -I ./incl_bonus
SRCS_DIR_B	= srcs_bonus
OBJS_DIR_B	= objs_bonus

SRCS_B	= $(wildcard $(SRCS_DIR_B)/*.cpp)
OBJS_B	= $(addprefix $(OBJS_DIR_B)/, $(SRCS_B:.cpp=.o))
DEPS_B	= $(OBJS_B:.o=.d)

################################################################################
#####                           GENERAL RULES PART                         #####
################################################################################

.PHONY: all clean fclean re

all		: $(NAME)

$(NAME)	: $(OBJS)
		$(CXX) -o $@ $^

objs/%.o: %.cpp
		@mkdir -p $(dir $@)
		${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@

clean	:
		$(RM) $(OBJS_DIR) $(OBJS_DIR_B)

fclean	:
		$(RM) $(OBJS_DIR) $(OBJS_DIR_B) $(NAME) $(NAME_B)

re		:
		$(RM) $(OBJS_DIR) $(NAME)
		make all

################################################################################
#####                            BONUS RULES PART                          #####
################################################################################

.PHONY: bonus reb

bonus			: $(NAME_B)

$(NAME_B)		: $(OBJS_B)
		$(CXX) -o $@ $^

objs_bonus/%.o	: %.cpp
		@mkdir -p $(dir $@)
		${CXX} ${CXXFLAGS} ${INCLUDES_B} -c $< -o $@

reb				:
		$(RM) $(OBJS_DIR_B) $(NAME_B)
		make bonus

################################################################################
#####                          CUSTOM ALIASES PART                         #####
################################################################################

.PHONY: run test run_bonus test_bonus

run			:
		@make --no-print-directory re
		@make --no-print-directory clean
		@clear;
		@echo '/connect 127.0.0.1 1111 1234'
		./${NAME} 1111 1234

test		:
		@make --no-print-directory re
		@make --no-print-directory clean
		@clear;
		@echo '/connect 127.0.0.1 1111 1234'
		valgrind --track-fds=yes -s ./${NAME} 1111 1234

run_bonus	:
		@make --no-print-directory reb
		@make --no-print-directory clean
		@clear;
		@echo '/connect 127.0.0.1 1111 1234'
		./${NAME_B} 1111 1234

test_bonus	:
		@make --no-print-directory reb
		@make --no-print-directory clean
		@clear;
		@echo '/connect 127.0.0.1 1111 1234'
		valgrind --track-fds=yes -s ./${NAME_B} 1111 1234

-include $(DEPS) $(DEPS_B)
