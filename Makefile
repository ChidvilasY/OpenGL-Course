# source files
SRCS := \
    Main.cpp

ifeq ($(OS),Windows_NT)
	include win.mak
else
	include linux.mak
endif