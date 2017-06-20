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
#ifdef __cplusplus
extern "C"{
#include <openlase/libol.h>
}
#else
#include <openlase/libol.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Leap.h>
#include <spline.h>

int main (int argc, char *argv[])
{
    OLRenderParams params;
    Leap::Controller controller;

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
    int frames = 0;

    while(1) {
        olLoadIdentity3();
        olLoadIdentity();
        olTranslate3(0, 0, -3);
        int color = (255<<8);
        int color2 = (255<<0);
        
        if(controller.isConnected()){
            Leap::Frame frame = controller.frame();
            Leap::HandList hands = frame.hands();
            std::vector<Leap::Finger> fvect;

            for(auto h : hands) {
                Leap::FingerList fingers = h.fingers();
                for(auto f : fingers) {
                    fvect.push_back(f);
                }
            }
            printf("Fingers: %d\n", fvect.size());

            std::sort(fvect.begin(), fvect.end(), [](Leap::Finger a, Leap::Finger b) {
                return a.stabilizedTipPosition().x < b.stabilizedTipPosition().x;   
            });

            tk::spline s;
            std::vector<double> X, Y;
            double lastX = -100000.0;
            for(auto f : fvect) {
                Leap::Vector v = frame.interactionBox().normalizePoint(f.stabilizedTipPosition(), false);
                if(v.x != lastX) {
                    X.push_back(2*v.x-1);
                    Y.push_back(2*v.y-1);
                    lastX = v.x;
                }
            }

            if(X.size() > 2) {
                s.set_points(X,Y);
                int div = 50;
                olBegin(OL_LINESTRIP);
                for(int i=0; i <= div; i++) {
                    double x = -1.0 + (2.0*i)/div;
                    double y = s(x);
                    if(std::abs(y) < 2.0) {
                        olVertex3(x, s(x), -1, color);
                    }
                }
                olEnd();

                for(auto f : fvect) {

                }
            }

            for(auto h : hands) {
                Leap::FingerList fingers = h.fingers();
                Leap::Vector p = frame.interactionBox().normalizePoint(h.stabilizedPalmPosition(), false);

                olBegin(OL_LINESTRIP);
                for(auto f : fingers) {
                    Leap::Vector v = frame.interactionBox().normalizePoint(f.stabilizedTipPosition(), false);
                    olVertex3(2*p.x-1, 2*p.y-1, -1, color2);
                    olVertex3(2*v.x-1, 2*v.y-1, -1, color2);
                }
                olEnd();
            }

        } else {
            olBegin(OL_LINESTRIP);
            olVertex3(-0.5, -0.5, -1, color);
            olVertex3(0.5, 0.5, -1, color);
            olEnd();

            olBegin(OL_LINESTRIP);
            olVertex3(-0.5, 0.5, -1, color);
            olVertex3(0.5, -0.5, -1, color);
            olEnd();
        }

        ftime = olRenderFrame(60);
        frames++;
        time += ftime;
    }

    olShutdown();
    exit (0);
}

