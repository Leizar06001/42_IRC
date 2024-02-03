NAME = irc_server

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCS = 	main.cpp					\
		srcs/ChannelList.class.cpp	\
		srcs/server.class.cpp		\
		srcs/Server.actions.cpp		\
		srcs/Server.cmd_ban.cpp		\
		srcs/Server.cmd_cap.cpp		\
		srcs/Server.cmd_invite.cpp	\
		srcs/Server.cmd_join.cpp	\
		srcs/Server.cmd_kick.cpp	\
		srcs/Server.cmd_mode.cpp	\
		srcs/Server.cmd_names.cpp	\
		srcs/Server.cmd_nick.cpp	\
		srcs/Server.cmd_part.cpp	\
		srcs/Server.cmd_ping.cpp	\
		srcs/Server.cmd_pong.cpp	\
		srcs/Server.cmd_privmsg.cpp	\
		srcs/Server.cmd_quit.cpp	\
		srcs/Server.cmd_topic.cpp	\
		srcs/Server.cmd_user.cpp	\
		srcs/Server.cmd_whois.cpp	\
		srcs/Server.files.cpp		\
		srcs/Server.in_out.cpp		\
		srcs/Server.parse.cpp		\
		srcs/Server.timeouts.cpp	\
		srcs/userList.class.cpp		\
		srcs/userInfos.class.cpp	\
		srcs/terminal.class.cpp

OBJ_DIR = objs
OBJS := $(SRCS:srcs/%=%)
OBJS := $(OBJS:%.cpp=$(OBJ_DIR)/%.o)
  # Strip 'srcs/' from the object file paths

all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@echo "Linking..."
	@$(CC) $(OBJS) -o $(NAME)
	@echo "Done !"

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
