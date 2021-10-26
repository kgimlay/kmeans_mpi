
#!/usr/bin/env bash


# script operation
MIN_K_POW=0
MAX_K_POW=10
K_POW_BASE=2
REPEAT=5
RUN_SEQ_LLOYD=TRUE
RUN_MPI_LLOYD=TRUE
RUN_SEQ_YIN=FALSE
RUN_MPI_YIN=FALSE

# mpi parameters
MIN_PROC=1
MAX_PROC=8

# required parameters
DATA_SET_PATH='./datasets/'
DATA_SET='M2.csv'
DATA_POINTS=1000000
DATA_DIM=2

# optional parameters
OUTPUT_PATH='./output/'
MAX_ITR=100000

# program call
PROG_CALL='./kmeans_main'


cd ../../.. # to root
make        # make to reflect most recent changes
if [ ! $? -eq 0 ]; then echo 'build failed' && exit
fi
echo ''

# run sequential Lloyds
if [ $RUN_SEQ_LLOYD == TRUE ]
then
  # iterate over k sizes
  for k in $(seq $MIN_K_POW $MAX_K_POW)
  do
    k=$(($K_POW_BASE**$k))
    echo 'Sequential Lloyds: k =' $k
    # repeat for averaging time
    for rep in $(seq $REPEAT)
    do
      # echo $rep '/' $REPEAT
      $PROG_CALL $DATA_SET_PATH$DATA_SET $DATA_POINTS $DATA_DIM SEQ_LLOYD $k -i $MAX_ITR -v 1
      if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
      fi
    done
    echo ''
  done
  echo ''
fi

# run mpi Lloyds
if [ $RUN_MPI_LLOYD == TRUE ]
then
  for numP in $(seq $MIN_PROC $MAX_PROC)
  do
    echo 'MPI Lloyds' $numP 'Process(es)'
    for k in $(seq $MIN_K_POW $MAX_K_POW)
    do
      k=$(($K_POW_BASE**$k))
      echo 'MPI Lloyds: k =' $k
      for rep in $(seq $REPEAT)
      do
        # echo $rep '/' $REPEAT
        mpiexec -np $numP $PROG_CALL $DATA_SET_PATH$DATA_SET $DATA_POINTS $DATA_DIM MPI_LLOYD $k -i $MAX_ITR -o $OUTPUT_PATH
        if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
        fi
      done
      echo ''
    done
    echo ''
  done
fi

# run sequential YinYang
if [ $RUN_SEQ_YIN == TRUE ]
then
  for k in $(seq $MIN_K_POW $MAX_K_POW)
  do
    k=$(($K_POW_BASE**$k))
    echo 'Sequential YinYang: k =' $k
    for rep in $(seq $REPEAT)
    do
      # echo $rep '/' $REPEAT
      $PROG_CALL $DATA_SET_PATH$DATA_SET $DATA_POINTS $DATA_DIM SEQ_YINYANG $k -i $MAX_ITR -o $OUTPUT_PATH
      if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
      fi
    done
    echo ''
  done
  echo ''
fi

# run mpi YinYang
if [ $RUN_SEQ_YIN == TRUE ]
then
  for numP in $(seq $MIN_PROC $MAX_PROC)
  do
    echo 'MPI YinYang' $numP 'Process(es)'
    for k in $(seq $MIN_K_POW $MAX_K_POW)
    do
      k=$(($K_POW_BASE**$k))
      echo 'MPI YinYang: k =' $k
      for rep in $(seq $REPEAT)
      do
        # echo $rep '/' $REPEAT
        mpiexec -np $numP $PROG_CALL $DATA_SET_PATH$DATA_SET $DATA_POINTS $DATA_DIM MPI_YINYANG $k -i $MAX_ITR -o $OUTPUT_PATH
        if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
        fi
      done
      echo ''
    done
    echo ''
  done
fi
