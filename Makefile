NAME = irc_server

SRCS =	source/Client.cpp \
		source/Message.cpp \
		source/Protocol.cpp \
		source/Reply.cpp \
		source/Server.cpp \
		source/Channel.cpp \
		source/command/PART.cpp \
		source/command/JOIN.cpp \
		source/command/NAMES.cpp \
		source/command/PRIVMSG.cpp \
		source/command/WHOIS.cpp \
		source/command/QUIT.cpp \
		source/command/USER.cpp \
		source/command/NICK.cpp \
		source/command/PING.cpp \
		source/command/PASS.cpp \
		source/command/KICK.cpp \
		source/command/INVITE.cpp \
		source/command/TOPIC.cpp \
		source/command/OPER.cpp \
		source/command/MODE.cpp \
		ft_irc.cpp \

OBJS = $(SRCS:%.cpp=$(OBJ)/%.o)

DEP = $(OBJS:%.o=%.d)

OBJ = ./obj

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -pedantic -g -Iinclude

RM = rm -f

all: $(NAME)

$(NAME):	$(OBJS)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

-include $(DEP)

$(OBJ)/%.o:	%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJ)/$(OBJS)

fclean: clean
	$(RM) $(NAME)
	rm -fr $(OBJ)

re: fclean $(NAME)

valgrind: $(NAME)
	valgrind --leak-check=full ./$(NAME)  8080 pw

run: $(NAME)
	./$(NAME) 8080 pw

.PHONY: all clean fclean re  valgrind run
