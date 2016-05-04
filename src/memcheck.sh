echo "Compiling and Running Valgrind"
g++ -std=c++11 -fopenmp test.cpp -I lists/ -o testmemcheck.out
valgrind --leak-check=full ./testmemcheck.out
rm testmemcheck.out
