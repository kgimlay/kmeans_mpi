# Author: Kevin Imlay

CC = mpicc
KMEANS_EXECUTABLE = kmeans_main
KMEANS_WRAPPER_EXECUTABLE = kmeans_wrapper
FLAGS = -O3 -Wall -std=c99 -pedantic -g -fopenmp
CFLAGS = -c -pedantic
CODE_INC_DIR = code/inc/
CODE_SRC_DIR = code/src/
OBJ_DIR = objects/


all: $(KMEANS_EXECUTABLE) $(KMEANS_WRAPPER_EXECUTABLE)

kmeans_wrapper_main.o: $(CODE_SRC_DIR)kmeans_wrapper_main.c $(CODE_INC_DIR)kmeans_wrapper_main.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)kmeans_wrapper_main.c -o $(OBJ_DIR)kmeans_wrapper_main.o

kmeans_main.o: $(CODE_SRC_DIR)kmeans_main.c $(CODE_INC_DIR)kmeans_main.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)kmeans_main.c -o $(OBJ_DIR)kmeans_main.o

utils.o: $(CODE_SRC_DIR)utils.c $(CODE_INC_DIR)utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)utils.c -o $(OBJ_DIR)utils.o

file_utils.o: $(CODE_SRC_DIR)file_utils.c $(CODE_INC_DIR)file_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)file_utils.c -o $(OBJ_DIR)file_utils.o

data_utils.o: $(CODE_SRC_DIR)data_utils.c $(CODE_INC_DIR)data_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)data_utils.c -o $(OBJ_DIR)data_utils.o

time_utils.o: $(CODE_SRC_DIR)time_utils.c $(CODE_INC_DIR)time_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)time_utils.c -o $(OBJ_DIR)time_utils.o

command_line_utils.o: $(CODE_SRC_DIR)command_line_utils.c $(CODE_INC_DIR)command_line_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)command_line_utils.c -o $(OBJ_DIR)command_line_utils.o

mpi_utils.o: $(CODE_SRC_DIR)mpi_utils.c $(CODE_INC_DIR)mpi_utils.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)mpi_utils.c -o $(OBJ_DIR)mpi_utils.o

seq_lloyd.o: $(CODE_SRC_DIR)seq_lloyd.c $(CODE_INC_DIR)seq_lloyd.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)seq_lloyd.c -o $(OBJ_DIR)seq_lloyd.o

mpi_lloyd.o: $(CODE_SRC_DIR)mpi_lloyd.c $(CODE_INC_DIR)mpi_lloyd.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)mpi_lloyd.c -o $(OBJ_DIR)mpi_lloyd.o

mpi_omp_lloyd.o: $(CODE_SRC_DIR)mpi_omp_lloyd.c $(CODE_INC_DIR)mpi_omp_lloyd.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)mpi_omp_lloyd.c -o $(OBJ_DIR)mpi_omp_lloyd.o

seq_yinyang.o: $(CODE_SRC_DIR)seq_yinyang.c $(CODE_INC_DIR)seq_yinyang.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)seq_yinyang.c -o $(OBJ_DIR)seq_yinyang.o

mpi_yinyang.o: $(CODE_SRC_DIR)mpi_yinyang.c $(CODE_INC_DIR)mpi_yinyang.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)mpi_yinyang.c -o $(OBJ_DIR)mpi_yinyang.o

mpi_omp_yinyang.o: $(CODE_SRC_DIR)mpi_omp_yinyang.c $(CODE_INC_DIR)mpi_omp_yinyang.h
	$(CC) $(CFLAGS) $(FLAGS) $(CODE_SRC_DIR)mpi_omp_yinyang.c -o $(OBJ_DIR)mpi_omp_yinyang.o

$(KMEANS_EXECUTABLE): kmeans_main.o \
	file_utils.o \
	command_line_utils.o \
	utils.o \
	data_utils.o \
	time_utils.o \
	mpi_utils.o \
	seq_lloyd.o \
	mpi_lloyd.o \
	mpi_omp_lloyd.o \
	seq_yinyang.o \
	mpi_yinyang.o \
	mpi_omp_yinyang.o
	$(CC) $(FLAGS) -o $(KMEANS_EXECUTABLE) $(OBJ_DIR)kmeans_main.o \
	$(OBJ_DIR)file_utils.o \
	$(OBJ_DIR)utils.o \
	$(OBJ_DIR)command_line_utils.o \
	$(OBJ_DIR)data_utils.o \
	$(OBJ_DIR)time_utils.o \
	$(OBJ_DIR)mpi_utils.o \
	$(OBJ_DIR)seq_lloyd.o \
	$(OBJ_DIR)mpi_lloyd.o \
	$(OBJ_DIR)mpi_omp_lloyd.o \
	$(OBJ_DIR)seq_yinyang.o \
	$(OBJ_DIR)mpi_yinyang.o \
	$(OBJ_DIR)mpi_omp_yinyang.o

$(KMEANS_WRAPPER_EXECUTABLE): kmeans_wrapper_main.o \
	utils.o
	$(CC) $(FLAGS) -o $(KMEANS_WRAPPER_EXECUTABLE) $(OBJ_DIR)kmeans_wrapper_main.o \
	$(OBJ_DIR)utils.o


clean:
	rm $(OBJ_DIR)*.o
	rm $(KMEANS_EXECUTABLE)
	rm $(KMEANS_WRAPPER_EXECUTABLE)
