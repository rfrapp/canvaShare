exe: 
	g++ server.cpp `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL_net -Iincludes -I/usr/local/include -L/usr/local/lib -w
mac:
	g++ server.cpp `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_net -Iincludes -I/usr/local/include -L/usr/local/lib -w
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
