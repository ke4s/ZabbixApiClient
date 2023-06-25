NAME = ZabbixApiClient

C++ = g++

FLAGS = -lboost_system -lboost_thread -lboost_date_time -lboost_chrono -lboost_regex -lboost_atomic -lboost_serialization -lboost_filesystem -lboost_log -lboost_log_setup -lboost_program_options -lboost_unit_test_framework -lboost_program_options -I /usr/include
SRC =	main.cpp
HEADERS = Headers/GetProgramOptions.hpp Headers/ZabbixApiClient.hpp Headers/utils.hpp Headers/PTreePrinter.hpp

all:	$(NAME) $(HEADERS)

ZabbixApiClient:	$(SRC) $(HEADERS)
	$(C++) -o $(NAME) $(SRC) $(FLAGS)

clean :
		rm -rf $(NAME)

fclean : clean
		@echo Cleaning %100

re : fclean all

.PHONY : all clean fclean re