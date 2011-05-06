maxflow: capacity_scaling.c
	gcc -o run capacity_scaling.c -lm
clean:
	rm -rf run *.o *.out output.txt
