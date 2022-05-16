#/*
#* NAME: Alec Trent                             USER ID: ajtrent
#* DUE DATE: 04/15/2022
#* PROGRAM ASSIGNMENT 5
#* FILE NAME: Makefile
#* PROGRAM PURPOSE:
#*	Compiles the files needed to execute the program
#* DAYS LATE: 1
#* LATE DAYS REMAINING: 2
#*/


CC       = c++
CFLAGS   = -g -O2 -Wno-write-strings -Wno-cpp -w
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I/local/eit-linux/apps/ThreadMentor/include
TMLIB    = /local/eit-linux/apps/ThreadMentor/Visual/libthreadclass.a
TMLIB_NV    = /local/eit-linux/apps/ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = thread.o thread-main.o boat-monitor.o
EXE_FILE = prog5

${EXE_FILE}: ${OBJ_FILE}
	${CC}	-o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB} -lpthread

thread.o: thread.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread.cpp

thread-main.o: thread-main.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread-main.cpp

boat-monitor.o: boat-monitor.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c boat-monitor.cpp

noVisual: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread
					
clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}

