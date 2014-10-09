exe: main.cpp
	g++ *.cpp src/*.cpp `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_gfx -Iincludes -I/usr/local/include -w
r:
	./a.out
clean:
	find . -name '*.out' -exec rm '{}' \;
push:
	make clean 
	git add . -A
	git commit 
	git push
pull:
	git pull 
