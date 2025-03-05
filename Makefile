all:
	gcc -o generate_tones generate_tones.c
	./generate_tones
	gcc -o piano piano.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_mixer
	

clean:
	rm -f piano
	rm -f generate_tones
	rm -rf *.wav
