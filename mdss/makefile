all:
	gcc main.c randys.c sha256.c -lpbc -lgmp -L. -l:libMerkleTree.a -o 1000.out
release:
	gcc main.c randys.c sha256.c -lpbc -lgmp -O2
clean:
	rm *.out