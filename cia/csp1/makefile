store: 
	gcc store.c ../utils/randys.c
proof_gen1:
	gcc proof_gen1.c ../utils/randys.c ../utils/sha256.c -lgmp
proof_verify1:
	gcc proof_verify1.c ../utils/randys.c ../utils/sha256.c -lgmp
clean:
	rm a.out
	find ../swap/csp1/blocks/ -type f | xargs rm -f
	find ../swap/csp2/blocks/ -type f | xargs rm -f
	rm ../swap/*.mp3.*
	rm ../swap/*.tmp.*
	rm ../swap/*.flac.*
	rm ../swap/*.mkv.*