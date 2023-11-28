debug: main.cpp
	g++ -Wall -g -o final_debug main.cpp
optimized: main.cpp
	g++ -Wall -O2 -o final_optimized main.cpp
clean:
	rm -f *.o final_debug final_optimized
