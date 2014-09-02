exe: main.cpp
	g++ *.cpp src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -Iincludes -w
r:
	./a.out
push:
	git add . -A
	git commit 
	git push
clean:
	rm ./a.out 
