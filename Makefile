assembler: main.o bitwise_utils.o  error_handler.o  file_utils.o  flow_controller.o ref_tables.o  row_analyzer.o
	gcc -g  main.o bitwise_utils.o  error_handler.o  file_utils.o  flow_controller.o ref_tables.o  row_analyzer.o -lm -o assembler


bitwise_utils.o: bitwise_utils.c
	gcc -Wall -ansi -c bitwise_utils.c -o bitwise_utils.o

error_handler.o: error_handler.c
	gcc -Wall -ansi -c error_handler.c -o error_handler.o

file_utils.o: file_utils.c
	gcc -Wall -ansi -c file_utils.c -o file_utils.o

flow_controller.o: flow_controller.c
	gcc -Wall -ansi -c flow_controller.c -o flow_controller.o

ref_tables.o: ref_tables.c
	gcc -Wall -ansi -c ref_tables.c -o ref_tables.o

row_analyzer.o: row_analyzer.c
	gcc -Wall -ansi -c row_analyzer.c -o row_analyzer.o

main.o: main.c
	gcc -Wall -ansi -c main.c -o main.o

clean:
	rm -rf main.o bitwise_utils.o  error_handler.o  file_utils.o  flow_controller.o ref_tables.o  row_analyzer.o assembler


