/*
        OpenLase - a realtime laser graphics toolkit

Copyright (C) 2009-2011 Hector Martin "marcan" <hector@marcansoft.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 or version 3.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <openlase/libol.h>

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <jack/jack.h>
#include <math.h>

void HSBToRGB(
    unsigned int inHue, unsigned int inSaturation, unsigned int inBrightness,
    unsigned int *oR, unsigned int *oG, unsigned int *oB )
{
    if (inSaturation == 0)
    {
        // achromatic (grey)
        *oR = *oG = *oB = inBrightness;
    }
    else
    {
        unsigned int scaledHue = (inHue * 6);
        unsigned int sector = scaledHue >> 8; // sector 0 to 5 around the color wheel
        unsigned int offsetInSector = scaledHue - (sector << 8);	// position within the sector
        unsigned int p = (inBrightness * ( 255 - inSaturation )) >> 8;
        unsigned int q = (inBrightness * ( 255 - ((inSaturation * offsetInSector) >> 8) )) >> 8;
        unsigned int t = (inBrightness * ( 255 - ((inSaturation * ( 255 - offsetInSector )) >> 8) )) >> 8;

        switch( sector ) {
        case 0:
            *oR = inBrightness;
            *oG = t;
            *oB = p;
            break;
        case 1:
            *oR = q;
            *oG = inBrightness;
            *oB = p;
            break;
        case 2:
            *oR = p;
            *oG = inBrightness;
            *oB = t;
            break;
        case 3:
            *oR = p;
            *oG = q;
            *oB = inBrightness;
            break;
        case 4:
            *oR = t;
            *oG = p;
            *oB = inBrightness;
            break;
        default:    // case 5:
            *oR = inBrightness;
            *oG = p;
            *oB = q;
            break;
        }
    }
}

/*
Simple demonstration, shows two cubes in perspective 3D.
*/

int main (int argc, char *argv[])
{
	OLRenderParams params;

	memset(&params, 0, sizeof params);
	params.rate = 48000;
	params.on_speed = 2.0/100.0;
	params.off_speed = 2.0/20.0;
	params.start_wait = 8;
	params.start_dwell = 3;
	params.curve_dwell = 0;
	params.corner_dwell = 8;
	params.curve_angle = cosf(30.0*(M_PI/180.0)); // 30 deg
	params.end_dwell = 3;
	params.end_wait = 7;
	params.snap = 1/100000.0;
	params.render_flags = RENDER_GRAYSCALE;

	if(olInit(3, 30000) < 0)
		return 1;

	olSetRenderParams(&params);

	float time = 0;
	float ftime;
	int i;

	int frames = 0;

	int hue = 0;
	int saturation = 255;
	int brightness = 255;

	while(1) {
		olLoadIdentity3();
		olLoadIdentity();
		olPerspective(60, 1, 1, 100);
		olTranslate3(0, 0, -3);
		olRotate3Z(time * M_PI * 0.3);

		hue = (hue + 1) % 255;

		unsigned int r,g,b;
		HSBToRGB(hue, saturation, brightness, &r, &g, &b);

		int color = (255<<8);
		int div = 30;

		olBegin(OL_LINESTRIP);
		olVertex3(1, 0, -1, color);
		for(i=1; i < div; i++) {
			float r = 2*M_PI*((float)i / (float)div);
			olVertex3(cos(r), sin(r), -1, color);
		}
		olVertex3(1, 0, -1, color);
		olEnd();

		float step = (2.0/5.0)*M_PI;
		olBegin(OL_LINESTRIP);
		olVertex3(1, 0, -1, color);
		olVertex3(cos(2*step), sin(2*step), -1, color);
		olVertex3(cos(4*step), sin(4*step), -1, color);
		olVertex3(cos(1*step), sin(1*step), -1, color);
		olVertex3(cos(3*step), sin(3*step), -1, color);
		olVertex3(1, 0, -1, color);
		olEnd();


		ftime = olRenderFrame(60);
		frames++;
		time += ftime;
		//printf("Frame time: %f, FPS:%f\n", ftime, frames/time);
	}

	olShutdown();
	exit (0);
}

