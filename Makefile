target:
	g++ -Wall -g -o final_debug main.cpp class_definitions.cpp class_structures.h headers.h
	g++ -Wall -O2 -o final_optimized main.cpp class_definitions.cpp class_structures.h headers.h

clean:
	rm -f *.o final_debug final_optimized final

