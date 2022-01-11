NAME		= webserv
CXX			= clang++
RM			= rm -f
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g
SRCS		= ./main.cpp ./cpp_srcs/Socket.cpp  ./cpp_srcs/Configuration.cpp  \
			  ./cpp_srcs/Request.cpp ./cpp_srcs/parser.cpp ./cpp_srcs/Server.cpp ./cpp_srcs/Response.cpp \
			  ./cpp_srcs/Client.cpp ./cpp_srcs/Cgi.cpp
OBJS		= $(SRCS:.cpp=.o)
HEADER		= ./inc/webserv.hpp ./inc/Socket.hpp ./inc/Configuration.hpp \
			  ./inc/Request.hpp ./inc/Server.hpp ./inc/Response.hpp ./inc/Client.hpp ./inc/CGI.hpp
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
