bridge: bridge.o
	gcc bridge.o -pthread -o bridge

bridge.o: bridge.c 
	gcc bridge.c -c -pthread

clean:
	rm *.o bridge