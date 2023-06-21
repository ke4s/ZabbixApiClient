NAME = ZabbixApiClient

C++ = g++

FLAGS = -lboost_system -lboost_thread -lboost_date_time -lboost_chrono -lboost_regex -lboost_atomic -lboost_serialization -lboost_filesystem -lboost_log -lboost_log_setup -lboost_program_options -lboost_unit_test_framework

SRC =	main.cpp \



all:	$(SRC_SERV) $(NAME)

ZabbixApiClient:	$(SRC)
	$(C++) $(FLAGS) $(SRC) -o $(NAME)

clean :
		rm -rf $(NAME)

fclean : clean
		@echo Cleaning %100

re : fclean all

.PHONY : all clean fclean re