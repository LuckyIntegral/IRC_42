CXX			= c++
NAME		= ircserv
NAME_B		= ircserv_bonus
RM			= rm -rf

CXXFLAGS	= -Wall -Wextra -Werror -MD -MP -g -std=c++98 -pedantic

MAKEFLAGS	= -j$(nproc)

INCLUDES	= -I ./incl
#SRCS	= ${SRCS_DIR}/main.cpp

SRCS_DIR	= srcs
OBJS_DIR	= objs
SRCS		= $(wildcard $(SRCS_DIR)/*.cpp)
OBJS	= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))
DEPS	= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.d))

SRCS_DIR_B	= srcs_bonus
OBJS_DIR_B	= objs_bonus
SRCS_B		= $(wildcard $(SRCS_DIR_B)/*.cpp)
OBJS_B	= $(addprefix $(OBJS_DIR_B)/, $(SRCS_B:.cpp=.o))
DEPS_B	= $(addprefix $(OBJS_DIR_B)/, $(SRCS_B:.cpp=.d))

all		: $(NAME)

bonus	: $(NAME_B)

run		:
		@make --no-print-directory re
		@make --no-print-directory clean
		@clear;
		@echo '/connect 127.0.0.1 1111 1234'
		./${NAME} 1111 1234

test	:
		@make --no-print-directory re
		@make --no-print-directory clean
		@clear;
		@echo '/connect 127.0.0.1 1111 1234'
		valgrind --track-fds=yes ./${NAME} 1111 1234

brun		:
		@make --no-print-directory re
		@make --no-print-directory clean
		@clear;
		@echo '/connect 127.0.0.1 1111 1234'
		./${NAME_B} 1111 1234

btest	:
		@make --no-print-directory re
		@make --no-print-directory clean
		@clear;
		@echo '/connect 127.0.0.1 1111 1234'
		valgrind --track-fds=yes ./${NAME_B} 1111 1234

$(NAME_B)	: $(OBJS_B)
		$(CXX) -o $@ $^

$(NAME)	: $(OBJS)
		$(CXX) -o $@ $^

objs/%.o: %.cpp
		@mkdir -p $(dir $@)
		${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@

objs_bonus/%.o: %.cpp
		@mkdir -p $(dir $@)
		${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@

clean	:
		$(RM) $(OBJS_DIR) $(OBJS_DIR_B)

fclean	:
		$(RM) $(OBJS_DIR) $(OBJS_DIR_B) $(NAME)

re		:
		$(RM) $(OBJS_DIR) $(NAME)
		make all

reb		:
		$(RM) $(OBJS_DIR_B) $(NAME_B)
		make bonus

-include $(DEPS)

.PHONY: all clean fclean re run test brun btest
