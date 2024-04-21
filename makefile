all: 
	g++ -Wall -Werror -pedantic-errors BruteForceMaxClique.cpp -o BruteForceMaxClique.exe
	g++ -Wall -Werror -pedantic-errors GraphGen.cpp -o Generator.exe
	g++ -Wall -Werror -pedantic-errors OriginalMaxClique.cpp -o Original.exe
	echo -O3 --param=sra-max-scalarization-size-Osize=4000
density:
	rm -f Density.exe
	g++ -Wall -Werror -pedantic-errors Density.cpp -o Density.exe
heuristic:
	g++ -Wall -Werror -g -pedantic-errors HeuristicMaxClique.cpp -o HeuristicMaxClique.exe
clean:
	rm -f *.exe