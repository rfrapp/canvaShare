exe: server.cpp
	g++ *.cpp `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_net -Iincludes -I/usr/local/include -w
r:
	./a.out
push:
	git add . -A
	git commit 
	git push

pull:
	git pull 
