# Author: Kevin Imlay

CC = mpicc
EXECUTABLE = kmeans
FLAGS = -O3 -Wall -std=c99 -pedantic
CFLAGS = -c
CODE_DIR = code
OBJ_DIR = objects


all: $(EXECUTABLE)

kmeans_mpi_main.o: $(CODE_DIR)/kmeans_mpi_main.c $(CODE_DIR)/kmeans_mpi_main.h $(CODE_DIR)/file_utils.h $(CODE_DIR)/parameters.h $(CODE_DIR)/lin_lloyd.h $(CODE_DIR)/command_line_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_DIR)/kmeans_mpi_main.c -o $(OBJ_DIR)/kmeans_mpi_main.o

file_utils.o: $(CODE_DIR)/file_utils.c $(CODE_DIR)/file_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_DIR)/file_utils.c -o $(OBJ_DIR)/file_utils.o

lin_lloyd.o: $(CODE_DIR)/lin_lloyd.c $(CODE_DIR)/lin_lloyd.h $(CODE_DIR)/parameters.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_DIR)/lin_lloyd.c -o $(OBJ_DIR)/lin_lloyd.o

command_line_utils.o: $(CODE_DIR)/command_line_utils.c $(CODE_DIR)/command_line_utils.h $(CODE_DIR)/helper.h $(CODE_DIR)/parameters.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_DIR)/command_line_utils.c -o $(OBJ_DIR)/command_line_utils.o

helper.o: $(CODE_DIR)/helper.c $(CODE_DIR)/helper.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_DIR)/helper.c -o $(OBJ_DIR)/helper.o

$(EXECUTABLE): kmeans_mpi_main.o file_utils.o lin_lloyd.o command_line_utils.o helper.o
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(OBJ_DIR)/kmeans_mpi_main.o $(OBJ_DIR)/file_utils.o $(OBJ_DIR)/lin_lloyd.o $(OBJ_DIR)/helper.o $(OBJ_DIR)/command_line_utils.o


clean:
	rm $(EXECUTABLE)
	rm $(OBJ_DIR)/*.o
