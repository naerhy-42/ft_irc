NAME = irc_server

SRCS = Client.cpp \
		ft_irc.cpp \
		Message.cpp \
		Protocol.cpp \
		Reply.cpp \
		Server.cpp \
		Channel.cpp

OBJS = $(SRCS:%.cpp=$(OBJ)/%.o)

DEP = $(OBJS:%.o=%.d)

OBJ = ./obj

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -pedantic

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
	rm -r $(OBJ)

re: fclean $(NAME)

.PHONY: all clean fclean re