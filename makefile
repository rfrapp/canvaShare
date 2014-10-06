exe: main.cpp
	g++ *.cpp src/*.cpp `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_gfx -Iincludes -w
r:
	./a.out
push:
	git add . -A
	git commit 
	git push

pull:
	git pull 
