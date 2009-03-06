# make for tcs stub symbol, used to verify which symbols are provided by TCS.
#
CC=tmcc

TARGET=pnx1500
HOST=nohost
ENDIAN=el
FP=fp64

all:tcs_sym.o

tcs_sym.o : tcs_sym_stub.c
	$(CC) -target $(TARGET) -host $(HOST) -$(ENDIAN) -$(FP) -c $^ -o tcs_sym_$(TARGET)_$(HOST)_$(ENDIAN)_$(FP).o 

clean:
	rm -fr *.o 
