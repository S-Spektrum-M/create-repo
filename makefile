FLAGS = -Wall -Werror -Wpedantic -O3
CXX = clang++

install: create-repo
	sudo mv create-repo /usr/bin/create-repo

create-repo: main.cpp
	$(CXX) main.cpp -o create-repo $(FLAGS)
