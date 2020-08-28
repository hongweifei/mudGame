#!/bin/bash


if [ ! -d "./app" ]
then
	mkdir app
fi


compile_linux()
{
	g++ 	-o app/a.out					\
			main.cpp console.cpp	 		\
			game/*.cpp	 					\
			-lncurses -lpanel
}


if [ ! $1 ]
then
	compile_linux
	exit
fi


if [ "$1" == "mingw" ] || [ "$1" == "Windows" ] || [ "$1" == "windows" ]
then
	x86_64-w64-mingw32-g++	-o app/a.exe			\
							main.cpp console.cpp 	\
							game/*.cpp				\
							-fstack-protector		\
							-lssp					\
							-lpanelw 				\
							-lncursesw

elif [ "$1" == "Linux" ] || [ "$1" == "linux" ]
then
	compile_linux

else
	echo "error: unrecognized command-line option '$1'"
fi





















		
		
