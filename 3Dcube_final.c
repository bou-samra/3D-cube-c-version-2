/****************************************************************************
 ** 3Dcube_final.c - display a 3D rotating cube                            **
 ** Copyright (c) 31/12/2022 Jason Bou-Samra [Paragon]                     **
 **                                                                        **
 ** This program is free software: you can redistribute it and/or modify   **
 ** it under the terms of the GNU General Public License as published by   **
 ** the Free Software Foundation, either version 3 of the License, or      **
 ** (at your option) any later version.                                    **
 **                                                                        **
 ** This program is distributed in the hope that it will be useful,        **
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of         **
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          **
 ** GNU General Public License for more details.                           **
 **                                                                        **
 ** You should have received a copy of the GNU General Public License      **
 ** along with this program.  If not, see <https://www.gnu.org/licenses/>. **
 ****************************************************************************/
 
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

int initSDL (void);
int shutdownSDL (void);
int rotate (void);
int render (void);

double rx, l, fs, c, s;
double x[8], y[8], z[8];            /* xyz coords */
double xt, yt, zt;                  /* temp xyz coords */
double vx[8], vy[8];                /* vertices */
bool quit = false;                  /* quit flag */

SDL_Window *window = NULL;
SDL_Renderer *sr;
SDL_Event event;

/* window paramaters */
#define WIDTH 640					/* pixels */
#define HEIGHT 512					/* pixels */
#define TITLE "3Dcube"					/* window title */
#define STEPS .05					/* resolution */

int main (void)
{
  initSDL ();
  SDL_RenderSetScale (sr, 2.0, 2.0);			/* scale x2 */
  rx = -0.1;
  l = 80;
  fs = 200;						/* scale factor */
  l = l / 2;
  while (quit == false)
    {

      while (SDL_PollEvent (&event))
	{
	  if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
	    {
	      quit = true;
	    }
	}
      if (quit)
	{
	  break;
	}

/* define cube */
      x[0] = -l; y[0] = -l; z[0] = -l;
      x[1] = -l; y[1] =  l; z[1] = -l;
      x[2] =  l; y[2] =  l; z[2] = -l;
      x[3] =  l; y[3] = -l; z[3] = -l;
      x[4] = -l; y[4] = -l; z[4] =  l;
      x[5] = -l; y[5] =  l; z[5] =  l;
      x[6] =  l; y[6] =  l; z[6] =  l;
      x[7] =  l; y[7] = -l; z[7] =  l;

      rx = rx + STEPS;
      c = cos (rx);
      s = sin (rx);

      rotate ();
      render ();

// SDL_Delay(50);
    }
  shutdownSDL ();					/* bye */
}

/* render */
int render ()
{
  /* foreground & background colours */
  SDL_SetRenderDrawColor (sr, 0x00, 0x10, 0x00, 255);	/* background */
  SDL_RenderClear (sr);					/* clear background */
  SDL_SetRenderDrawColor (sr, 0x00, 0xF0, 0x00, 255);	/* set pen colour*/

  /* draw cube */
  SDL_RenderDrawLine (sr, vx[0], vy[0], vx[1], vy[1]);
  SDL_RenderDrawLine (sr, vx[1], vy[1], vx[2], vy[2]);
  SDL_RenderDrawLine (sr, vx[2], vy[2], vx[3], vy[3]);
  SDL_RenderDrawLine (sr, vx[3], vy[3], vx[0], vy[0]);

  SDL_RenderDrawLine (sr, vx[4], vy[4], vx[5], vy[5]);
  SDL_RenderDrawLine (sr, vx[5], vy[5], vx[6], vy[6]);
  SDL_RenderDrawLine (sr, vx[6], vy[6], vx[7], vy[7]);
  SDL_RenderDrawLine (sr, vx[7], vy[7], vx[4], vy[4]);

  SDL_RenderDrawLine (sr, vx[0], vy[0], vx[4], vy[4]);
  SDL_RenderDrawLine (sr, vx[3], vy[3], vx[7], vy[7]);
  SDL_RenderDrawLine (sr, vx[1], vy[1], vx[5], vy[5]);
  SDL_RenderDrawLine (sr, vx[2], vy[2], vx[6], vy[6]);

  SDL_RenderPresent (sr);
}

/* rotation routine */
int rotate ()
{
  for (int np = 0; np < 8; np++)
    {

/* rotation around x axis */
      yt = y[np];
      y[np] = c * yt - s * z[np];
      z[np] = s * yt + c * z[np];

/* rotation around y axis */
      xt = x[np];
      x[np] = c * xt + s * z[np];
      z[np] = -s * xt + c * z[np];

/* rotation around z axis */
      xt = x[np];
      x[np] = c * xt - s * y[np];
      y[np] = s * xt + c * y[np];

/* now project into 2D */
      vx[np] = (x[np] * fs) / (z[np] + fs) + 160;
      vy[np] = (y[np] * fs) / (z[np] + fs) + 128;
    }
}

/* initialise SDL */
int initSDL ()
{
  SDL_Init (SDL_INIT_EVERYTHING);

/* new window */
  window = SDL_CreateWindow (TITLE,			/* title */
			     SDL_WINDOWPOS_UNDEFINED,	/* x pos */
			     SDL_WINDOWPOS_UNDEFINED,	/* w pos */
			     WIDTH,			/* width */
			     HEIGHT,			/* height */
			     0);			/* Additional flag(s) */

  /* deal with error */
  if (window == NULL)
    {
      fprintf (stderr, "SDL window failed to initialise: %s\n",
	       SDL_GetError ());
      return 1;
    }

/* creater renderer */
  sr = SDL_CreateRenderer (window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

/* shutdown routine */
int shutdownSDL ()
{
  SDL_DestroyWindow (window);				/* free memory */
  SDL_Quit ();						/* shutdown entire sdl sub-systems */
  return 0;
}

/* message */
char author[] = { "3d vector graphics demo by jason bou-samra" };
const char* text = "Amiga by Commodore all the way";
