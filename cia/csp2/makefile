store: 
	gcc store.c ../utils/randys.c
challenge:
	gcc challenge.c ../utils/randys.c -lgmp
proof_gen2:
	gcc proof_gen2.c ../utils/randys.c ../utils/sha256.c -lgmp
proof_verify2:
	gcc proof_verify2.c ../utils/randys.c ../utils/sha256.c -lgmp
clean:
	rm a.out
	find ../swap/csp1/blocks/ -type f | xargs rm -f
	find ../swap/csp2/blocks/ -type f | xargs rm -f
	rm ../swap/*.mp3.*
	rm ../swap/*.tmp.*
	rm ../swap/*.flac.*