NAME = webserv

SRCS = ./main.cpp ./socket/Socket.cpp  ./111/Configuration.cpp ./111/parser.cpp

HEADER = ./webserv.hpp ./socket/Socket.hpp ./111/Configuration.hpp

OBJS = $(SRCS:cpp=o)

%.o:		%.cpp $(HEADER)
			${CLANG} $(CPPFLAGS) -c $< -o ${<:.cpp=.o}

CLANG =		clang++
CPPFLAGS =	-Wall -Wextra -Werror -std=c++98
RM =		rm -f

all :		${NAME}

$(NAME) :	$(OBJS) 
			$(CLANG) $(CPPFLAGS) -o ${NAME} $(OBJS)

clean :		
			$(RM) $(OBJS)

fclean 	:	clean
			$(RM) ${NAME}

re :		fclean all

.PHONY :	all clean fclean re 