NAME = irc_server

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCS = 	main.cpp \
		srcs/server.class.cpp	\
		srcs/userList.class.cpp	\
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
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
