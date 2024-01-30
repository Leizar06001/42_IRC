NAME = irc_server

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCS = 	main.cpp \
		srcs/server.class.cpp	\
		srcs/userList.class.cpp	\
		srcs/userInfos.class.cpp	\
		srcs/terminal.class.cpp

OBJS = $(SRCS:%.cpp=%.o)

all: $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME):$(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
