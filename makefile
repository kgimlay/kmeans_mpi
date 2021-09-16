# Author: Kevin Imlay

CC = mpicc
EXECUTABLE = kmeans
FLAGS = -O3 -Wall -std=c99 -pedantic -g
CFLAGS = -c
CODE_INC_DIR = code/inc/
CODE_SRC_DIR = code/src/
OBJ_DIR = objects/


all: $(EXECUTABLE)

kmeans_mpi_main.o: $(CODE_SRC_DIR)kmeans_mpi_main.c $(CODE_INC_DIR)kmeans_mpi_main.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)kmeans_mpi_main.c -o $(OBJ_DIR)kmeans_mpi_main.o

utils.o: $(CODE_SRC_DIR)utils.c $(CODE_INC_DIR)utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)utils.c -o $(OBJ_DIR)utils.o

file_utils.o: $(CODE_SRC_DIR)file_utils.c $(CODE_INC_DIR)file_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)file_utils.c -o $(OBJ_DIR)file_utils.o

data_utils.o: $(CODE_SRC_DIR)data_utils.c $(CODE_INC_DIR)data_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)data_utils.c -o $(OBJ_DIR)data_utils.o

command_line_utils.o: $(CODE_SRC_DIR)command_line_utils.c $(CODE_INC_DIR)command_line_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)command_line_utils.c -o $(OBJ_DIR)command_line_utils.o

lin_lloyd.o: $(CODE_SRC_DIR)lin_lloyd.c $(CODE_INC_DIR)lin_lloyd.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)lin_lloyd.c -o $(OBJ_DIR)lin_lloyd.o

mpi_lloyd.o: $(CODE_SRC_DIR)mpi_lloyd.c $(CODE_INC_DIR)mpi_lloyd.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)mpi_lloyd.c -o $(OBJ_DIR)mpi_lloyd.o

$(EXECUTABLE): kmeans_mpi_main.o file_utils.o lin_lloyd.o command_line_utils.o \
	utils.o data_utils.o mpi_lloyd.o
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(OBJ_DIR)kmeans_mpi_main.o \
	$(OBJ_DIR)file_utils.o $(OBJ_DIR)lin_lloyd.o $(OBJ_DIR)utils.o \
	$(OBJ_DIR)command_line_utils.o $(OBJ_DIR)data_utils.o $(OBJ_DIR)mpi_lloyd.o


clean:
	rm $(OBJ_DIR)*.o
	rm $(EXECUTABLE)
