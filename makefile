all: 
	g++ -Wall -Werror -pedantic-errors -O3 --param=sra-max-scalarization-size-Osize=4000 OriginalMaxClique.cpp -o Original.exe
	echo -O3 --param=sra-max-scalarization-size-Osize=4000
original:
	g++ -Wall -Werror -pedantic-errors OriginalMaxClique.cpp -o Original.exe
verifier:
	g++ -Wall -Werror -pedantic-errors Verifier.cpp -o Verifier.exe
density:
	g++ -Wall -Werror -pedantic-errors Density.cpp -o Density.exe
heuristic:
	g++ -Wall -Werror -pedantic-errors -pedantic-errors -O3 --param=sra-max-scalarization-size-Osize=4000 HeuristicMaxClique.cpp -o HeuristicMaxClique.exe
generator:
	g++ -Wall -Werror -pedantic-errors GraphGen.cpp -o Generator.exe
bruteforce:
	g++ -Wall -Werror -pedantic-errors -O3 --param=sra-max-scalarization-size-Osize=4000 BruteForceMaxClique.cpp -o BruteForceMaxClique.exe
clean:
	rm -f *.exe