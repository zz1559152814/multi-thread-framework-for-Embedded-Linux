#CC := arm-linux-gcc
CC := gcc
INCLUDE := ./include
SRC 	:= ./src

THREAD_SRC		:= ${SRC}/thread
THREAD_INCLUDE 	:= ${INCLUDE}/thread

SERIAL_SRC		:= ${SRC}/serial
SERIAL_INCLUDE 	:= ${INCLUDE}/serial

ROUTE_SRC		:= ${SRC}/Route
ROUTE_INCLUDE	:= ${INCLUDE}/Route

GENER_FUNC_SRC		:= ${SRC}/general_func
GENER_FUNC_INCLUDE	:= ${INCLUDE}/general_func

ALL_INCLUDE := INCLUDE
ALL_INCLUDE := /home/dream/codes/2440_code
ALL_INCLUDE := ${THREAD_INCLUDE}
ALL_INCLUDE := ${SERIAL_INCLUDE}
ALL_INCLUDE := ${ROUTE_INCLUDE}
ALL_INCLUDE := ${GENER_FUNC_INCLUDE}

# OBJ := netsend.o serialsend.o serialrecv.o realspeed.o emergency.o handle.o


OBJ := netsend.o emergency.o serialsend.o serial.o \
		serialrecv.o realspeed.o handle.o mymath.o mecanum.o \
	
main: ${SRC}/main.c ${OBJ}
	${CC} -I ${INCLUDE} $^ -lpthread -lm -o $@
	rm *.o
	cscopeupdate
	
netsend.o: $(THREAD_SRC)/netsend.c  
	${CC} -c  -I ${INCLUDE} $^ -o $@

serialsend.o: $(THREAD_SRC)/serialsend.c 
	${CC} -c -g -I ${ALL_INCLUDE} $^ -lserial.o -o $@


serialrecv.o: $(THREAD_SRC)/serialrecv.c
	${CC} -c -g -I ${ALL_INCLUDE} $^ -lserial.o -o $@

realspeed.o: $(THREAD_SRC)/realspeed.c
	${CC} -lm -c -g $< -o $@

emergency.o: $(THREAD_SRC)/emergency.c
	${CC} -c -g $< -o $@

handle.o: $(THREAD_SRC)/handle.c 
	${CC} -c -g -I ${ALL_INCLUDE} -L mecanum.o $^ -o $@

serial.o : ${SERIAL_SRC}/serial.c
	${CC} -c -I ${ALL_INCLUDE} $^ -o $@	

mecanum.o: ${ROUTE_SRC}/mecanum.c 
	${CC} -c -I ${ALL_INCLUDE} $^ -lm -lmymath.o -o $@	

mymath.o : ${GENER_FUNC_SRC}/mymath.c 
	${CC} -c -I ${ALL_INCLUDE} $^ -lm  -o $@		
