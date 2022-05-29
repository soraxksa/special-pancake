


main: test main.c mutations.c mutations.h
	gcc main.c mutations.c -o main


test: test.c  
	gcc test.c -o test


clean:
	rm test main 
