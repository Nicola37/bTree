all: bTree
	
bTree: bTree.h bTree.cpp bTree_main.cpp
	g++ bTree.h bTree.cpp bTree_main.cpp -o bTree
	
clean:
	rm -rf bTree.exe
