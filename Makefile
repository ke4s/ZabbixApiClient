NAME = ZabbixApiClient

C++ = g++

FLAGS = -lboost_system -lboost_thread -lboost_date_time -lboost_chrono -lboost_regex -lboost_atomic -lboost_serialization -lboost_filesystem -lboost_log -lboost_log_setup -lboost_program_options -lboost_unit_test_framework -lboost_program_options -I /usr/include
SRC =	main.cpp Lib/GetProgramOptions.cpp Lib/PrepareRequestFromOpt.cpp Lib/PTreePrinter.cpp Lib/ZabbixApiClient.cpp
HEADERS = Lib/GetProgramOptions.hpp Lib/PrepareRequestFromOpt.hpp Lib/PTreePrinter.hpp Lib/ZabbixApiClient.hpp

all: $(SRC) $(HEADERS) $(NAME)

ZabbixApiClient:	$(SRC) $(HEADERS)
	$(C++) -o $(NAME) $(SRC) $(FLAGS)

clean :
		rm -rf $(NAME)

fclean : clean


re : fclean all

.PHONY : all clean fclean re