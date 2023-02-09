NAME = ft_irc
NAME_BONUS = ft_irc_bonus

SRCS =	source/ft_irc.cpp \
		source/Channel.cpp \
		source/Client.cpp \
		source/ClientMessage.cpp \
		source/Modes.cpp \
		source/Protocol.cpp \
		source/Server.cpp \
		source/ServerReplies.cpp \
		source/command/AWAY.cpp \
		source/command/DIE.cpp \
		source/command/INVITE.cpp \
		source/command/JOIN.cpp \
		source/command/KICK.cpp \
		source/command/KILL.cpp \
		source/command/MODE.cpp \
		source/command/MOTD.cpp \
		source/command/NAMES.cpp \
		source/command/NICK.cpp \
		source/command/NOTICE.cpp \
		source/command/OPER.cpp \
		source/command/PART.cpp \
		source/command/PASS.cpp \
		source/command/PING.cpp \
		source/command/PRIVMSG.cpp \
		source/command/QUIT.cpp \
		source/command/TOPIC.cpp \
		source/command/USER.cpp \
		source/command/WHOIS.cpp\


SRCS_BONUS =	source/ft_irc.cpp \
		source/Channel.cpp \
		source/Client.cpp \
		source/ClientMessage.cpp \
		source/Modes.cpp \
		source/Protocol.cpp \
		source/Server.cpp \
		source/ServerReplies.cpp \
		source/command/AWAY.cpp \
		source/command/DIE.cpp \
		source/command/INVITE.cpp \
		source/command/JOIN.cpp \
		source/command/KICK.cpp \
		source/command/KILL.cpp \
		source/command/MODE.cpp \
		source/command/MOTD.cpp \
		source/command/NAMES.cpp \
		source/command/NICK.cpp \
		source/command/NOTICE.cpp \
		source/command/OPER.cpp \
		source/command/PART.cpp \
		source/command/PASS.cpp \
		source/command/PING.cpp \
		source/command/PRIVMSG.cpp \
		source/command/QUIT.cpp \
		source/command/TOPIC.cpp \
		source/command/USER.cpp \
		source/command/WHOIS.cpp\

OBJS = $(SRCS:%.cpp=$(OBJ)/%.o)
OBJS_BONUS = $(SRCS_BONUS:%.cpp=$(OBJ_BONUS)/%.o)


DEP = $(OBJS:%.o=%.d)
DEP_BONUS = $(OBJS_BONUS:%.o=%.d)


OBJ = ./obj
OBJ_BONUS = ./obj_BONUS


CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -pedantic -g -Iinclude

RM = rm -f

all: $(NAME)

bonus: $(NAME_BONUS)


$(NAME):	$(OBJS)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(NAME_BONUS):	$(OBJS_BONUS)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

-include $(DEP)

$(OBJ)/%.o:	%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJ_BONUS)/%.o:	%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ -c $<


clean:
	$(RM) $(OBJ)/$(OBJS)
	$(RM) $(OBJ_BONUS)/$(OBJS_BONUS)

fclean: clean
	$(RM) $(NAME)
	rm -fr $(OBJ)
	$(RM) $(NAME_BONUS)
	rm -fr $(OBJ_BONUS)

re: fclean $(NAME)
	fclean $(NAME_BONUS)


valgrind: $(NAME)
	valgrind --leak-check=full ./$(NAME) 8080 pw

.PHONY: all clean fclean re bonus
