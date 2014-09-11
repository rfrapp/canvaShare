exe: main.cpp
	g++ *.cpp src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -Iincludes -w
r:
	./a.out
push:
	rm ./a.out 
	git add . -A
	git commit 
	git push
clean:

pull:
	git pull 
