#! /bin/make
# :vim: set noexpandtab:

CFLAGS = -lm `sdl2-config --cflags --libs`

3Dcube_final: 3Dcube_final.c
	$(CC) -o 3Dcube_final 3Dcube_final.c $(CFLAGS)

