NAME = dbtest

C++ = g++	

FLAGS = -lmysqlcppconn
SRC =	main.cpp

all: $(SRC) $(NAME)

$(NAME):	$(SRC)
	$(C++) -o $(NAME) $(SRC) $(FLAGS)

clean :
		rm -rf $(NAME)

fclean : clean


re : fclean all

.PHONY : all clean fclean re