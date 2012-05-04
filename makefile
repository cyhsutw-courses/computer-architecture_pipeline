PARA = -o
OBJ = ./Source/main.c
output: $(OBJ)
	gcc $(PARA) single_cycle $(OBJ)
clean:
	rm single_cycle
	rm *.rpt 