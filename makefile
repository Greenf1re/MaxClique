all: 
	g++ -Wall -Werror -pedantic-errors BruteForceMaxClique.cpp -o BruteForceMaxClique.exe
	g++ -Wall -Werror -pedantic-errors GraphGen.cpp -o Generator.exe
	g++ -Wall -Werror -pedantic-errors OriginalMaxClique.cpp -o Original.exe
	echo -O3 --param=sra-max-scalarization-size-Osize=4000
clean:
	rm -f *.exe