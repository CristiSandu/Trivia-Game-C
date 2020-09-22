build:trivia.c
	gcc trivia.c -Wall -o trivia -lncurses
run:
	./trivia myfile1.txt myfile2.txt myfile3.txt myfile4.txt myfile5.txt myfile6.txt myfile7.txt myfile8.txt myfile9.txt myfile10.txt
clean:
	rm -rf trivia 
