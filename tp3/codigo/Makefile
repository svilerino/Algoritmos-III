RM=rm
.PHONY: clean

all:
	make -C sol_exacta
	make -C bqlocal

clean:
	- $(RM) -rf */*.o */*~ *~
	make -C sol_exacta clean
	make -C bqlocal clean

