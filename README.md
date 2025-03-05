## Piano Game
A little piano simulator written in C using SDL2

It uses square waves to sound like an 8bit game. The code to generate sine waves is in `generate_tones.c`, commented out. You can just uncomment it and comment the square wave block to get sine waves. 

# Instructions
- Download and unzip the project
- There is a Makefile, so all you need to do is: in the project directory, run `make`. This will generate the .wav files and compile the code.
- Then run `./piano`
- `q` quits the program. Good luck