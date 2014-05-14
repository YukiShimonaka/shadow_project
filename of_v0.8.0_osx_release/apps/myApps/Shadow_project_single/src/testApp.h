#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>				// fprintf(), stderr
#include <stdlib.h>				// malloc(), free(), atexit()
#include <time.h>
#include <math.h>
#ifndef __APPLE__
#include <GL/gl.h>
#  include <GL/glut.h>
#  ifdef GL_VERSION_1_2
#    include <GL/glext.h>
#  endif
#else
#  include <GLUT/glut.h>
#  include <OpenGL/gl.h>
#  include <OpenGL/glext.h>
#endif
#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>			// arParamDisp()
#include <AR/ar.h>

const int CAM_WIDTH = 640;
const int CAM_HEIGHT = 480;
const int SNOW = 100; // the number of snow
const int GR = 170;

class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    static bool draw_flag;

    ofVideoGrabber cam2;
    
    ofxCvColorImage color2;
    ofxCvGrayscaleImage gray2;
    ofxCvGrayscaleImage	backGround;
        
    int cap_thresh;
    double pre_patt_trans[3];
    double rotX, rotY, rotZ;
};

class snow
{
public:
    void draw(void);
    void flow_right(void);
    void flow_left(void);
    
    static int             down_count[SNOW];
    static int             snowxpos_flg[SNOW];
    static int             snowypos_flg[SNOW];
    static float           snow_xpos[SNOW];
    static float           snow_ypos[SNOW];
    static float           xflow[SNOW];
    static float           yflow[SNOW];
    static float           snow_noise[SNOW];
    static int             l;
};

class ground
{
public:
    void flag(float w, float h);
    void draw(void);
    
    static int             gr_count;
    static int             gr_flg;
    static double          groot_count;
    static int             draw_flg;
    
private:
    static double          gr_xpos;
    static double          gr_ypos;
    static float           copy_array_right[GR];
    static float           copy_array_left[GR];
};

class grass {
public:
    void draw();
    
    static int flg;
    static float w;
    static float h;

private:
    void grass0(float x, float y);
    void grass1(float x, float y);
    void grass1_left(float x, float y);
    void grass1_right(float x, float y);

    float a = 40;
    float p = 84;
};

class tree{
public:
    void main(int x, int y, float a, int b, float w, float h);
    void root(int x, int y, float a, float w, float h);
    
};

class fan{
public:
    void setup(float w, float h);
};