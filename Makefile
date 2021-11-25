NAME		= webserv
CXX			= clang++
RM			= rm -f
CXXFLAGS	= -Wall -Wextra #-Werror -std=c++98 -g
SRCS		= ./main.cpp ./socket/Socket.cpp  ./Configuration/Configuration.cpp  ./Request/Request.cpp ./parser.cpp ./Server/Server.cpp ./Response/Response.cpp
OBJS		= $(SRCS:.cpp=.o)
HEADER		= ./inc/webserv.hpp ./socket/Socket.hpp ./Configuration/Configuration.hpp ./Request/Request.hpp ./Server/Server.hpp ./Response/Response.hpp
INC			= inc
%.o:		%.cpp $(HEADER)
			$(CXX) $(CXXFLAGS) -c $<  -o $(<:.cpp=.o) -I inc
			
all:		$(NAME) 
			@echo "\033[32mWebserv ready\033[0m"

$(NAME):	$(OBJS) $(HEADER) Makefile
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) 

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY :	all clean fclean re
