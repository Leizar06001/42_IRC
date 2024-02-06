NAME = irc_bot

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCS = 	srcs/main.cpp		\
		srcs/Bot.class.cpp	\
		srcs/Bot.cmds.cpp	\
		srcs/Bot.connect.cpp	\
		srcs/Bot.forbidden_words.cpp	\
		srcs/Bot.parse.cpp	\
		srcs/Bot.read.cpp	\
		prt.cpp

OBJ_DIR = objs
OBJS := $(SRCS:srcs/%=%)
OBJS := $(OBJS:%.cpp=$(OBJ_DIR)/%.o)
  # Strip 'srcs/' from the object file paths

all: pre-build-message $(NAME) post-build-message

pre-build-message:
	@echo "\033[2J\033[H"
	@echo "\033[31m╔═══════════════════════════════════════════╗"
	@echo "\033[31m║                                           ║"
	@echo "\033[31m║        \033[1m\033[33m (ﾟ◥益◤ﾟ) I͉ͪ͋͐̿R̟̗̜̘̠̿C̹̟ͫ͋̎ ̰̩̲͍̲̌͊ͯ̏̚b̫͙o͖̥̼̻͖͋t̖̞̄  (ʘ言ʘ╬) \033[0m\033[31m        ║"
	@echo "\033[31m║                                           ║"
	@echo "\033[31m╚═══════════════════════════════════════════╝\033[0m"

post-build-message:
	@echo "\033[31m╔═══════════════════════════════════════════╗"
	@echo "\033[31m║               \033[33m\033[1m  ALL GOOD  \033[0m\033[31m                ║"
	@echo "\033[31m╚═══════════════════════════════════════════╝\033[0m"

$(OBJ_DIR)/%.o: srcs/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "\033[31mCompiling $<...\033[0m"
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@echo "\033[33mLinking...\033[0m"
	@$(CC) $(OBJS) -o $(NAME)
	@echo "\033[33mDone !\033[0m"

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:
