all:
	gcc -o piano piano.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_mixer

clean:
	rm -f piano
