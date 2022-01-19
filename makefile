all: subnet

subnet: 
	gcc subnet.c -o subcalc -lm
clean: 
	rm -rf subcalc



