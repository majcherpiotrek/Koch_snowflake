all:
	@echo -n "Compiling Koch_snowflake ... "
		g++ -o Koch_snowflake Koch.cpp -g -Wall -lGL -lGLU -lglut  

test: Koch_snowflake
	@echo -n "Test ... "
	./Koch_snowflake

clean: 
	@echo -n "Cleaning the directory ... "
	rm Koch_snowflake
