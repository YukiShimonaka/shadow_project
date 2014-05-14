#include "testApp.h"

// ============================================================================
//	Constants and types.
// ============================================================================
#ifdef _WIN32
char			*vconf = "/Users/shimonaka/Documents/github/of_v0.8.0_osx_release/apps/myApps/Shadow_project_single/bin/data/WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

char           *cparam_name = "/Users/shimonaka/Documents/github/of_v0.8.0_osx_release/apps/myApps/Shadow_project_single/bin/data/camera_para.dat";
ARParam         cparam;

const char *patt_name = "/Users/shimonaka/Documents/github/of_v0.8.0_osx_release/apps/myApps/Shadow_project_single/bin/data/patt.hiro";
int             patt_id;
double          patt_width     = 40.0;
double          patt_center[2] = {0.0, 0.0};
double          patt_trans[3][4];

int             k;

int             cog_thresh = 50;

// mainloop time of doing count
int do_count = 0;
// as cording count
int test_count = 0;
// as cording flag
int ar_flag = 0;

bool testApp::draw_flag = FALSE;
double mk_posx, mk_posy;
double mk_svpos[2];

// --------------------------------------------------------------
//  Members
// --------------------------------------------------------------
int snow::down_count[SNOW];
int snow::snowxpos_flg[SNOW];
int snow::snowypos_flg[SNOW];
float snow::snow_xpos[SNOW];
float snow::snow_ypos[SNOW];
float snow::xflow[SNOW];
float snow::yflow[SNOW];
float snow::snow_noise[SNOW];
int snow::l = 0;

int ground::gr_count = 0;
int ground::gr_flg = 0;
double ground::groot_count = 0;
int ground::draw_flg = 0;
double ground::gr_xpos = 0;
double ground::gr_ypos = 0;
float ground::copy_array_right[];
float ground::copy_array_left[];

int grass::flg = 0;
float grass::w = 0;
float grass::h = 0;

void snow::draw()
{
    for (snow::l = 0; snow::l < SNOW; snow::l++) {
        if (ar_flag == 1 && snow::snowxpos_flg[snow::l] == 0) {
            snow::snow_xpos[snow::l] = ofRandom(-500, ofGetWidth() + 500);
            snow::snow_noise[snow::l] = ofRandom(-3.0, 0);
            snow::snowxpos_flg[snow::l] = 1;
        }
        if (ar_flag == 1 && snow::snowypos_flg[snow::l] == 0) {
            snow::snow_ypos[snow::l] = ofRandom(0, ofGetHeight());
            snow::snowypos_flg[snow::l] = 1;
        }
        
        if (ar_flag == 1 && snow::snowxpos_flg[snow::l] == 1) {
            ofPushMatrix();
            snow::xflow[snow::l] += (patt_trans[0][1] * 20);
            snow::yflow[snow::l] += fabs(patt_trans[0][1] * 5);
            ofTranslate(snow::snow_xpos[snow::l] + snow::xflow[snow::l], snow::snow_ypos[snow::l] + snow::down_count[snow::l] + snow::yflow[snow::l], 0.0);
            ofSetColor(0);
            ofCircle(0, 0, 3);
            ofPopMatrix();
            
            if ((snow::snow_ypos[snow::l] + snow::down_count[snow::l]) <= ofGetHeight()) {
                snow::down_count[snow::l] = snow::down_count[snow::l] + 5 + snow::snow_noise[snow::l];
            }
            else if ((snow::snow_ypos[snow::l] + snow::down_count[snow::l]) > ofGetHeight()) {
                snow::down_count[snow::l] = 0;
                snow::snowxpos_flg[snow::l] = 0;
                snow::snow_ypos[snow::l] = 0;
                snow::xflow[snow::l] = 0;
                snow::yflow[snow::l] = 0;
                snow::snow_noise[snow::l] = 0;
            }
        }
    }
}

void ground::draw(){
    ofSetColor(0);
    float array_right[ground::draw_flg + 2];
    float array_left[ground::draw_flg + 2];
    
    for (int n = 0; n < ground::draw_flg; n++) {
        array_right[n] = ground::copy_array_right[n];
        array_left[n] = ground::copy_array_left[n];
    }
    
    if (ground::draw_flg == 0) {
        array_right[ground::draw_flg] = 0;
        array_left[ground::draw_flg] = 0;
    }
    else{
        array_right[ground::draw_flg] = ofRandom(-3, 3);
        array_right[ground::draw_flg + 1] = ofRandom(0, 0);
        array_left[ground::draw_flg] = ofRandom(-3, 3);
        array_left[ground::draw_flg + 1] = ofRandom(0, 0);
    }
    
    float rapid = 16; // landing speed
    float p = 80; // positioning
    
    for (int l = 0; l < ground::draw_flg + 1; l++) {
        ofBeginShape();
        ofVertex(ground::gr_xpos + rapid * l, ground::gr_ypos + p + array_right[l]);
        ofVertex(ground::gr_xpos + rapid * (l + 1), ground::gr_ypos + p + array_right[l + 1]);
        ofVertex(ground::gr_xpos + rapid * (l + 1), ofGetHeight());
        ofVertex(ground::gr_xpos + rapid * l, ofGetHeight());
        ofEndShape();

        ofBeginShape();
        ofVertex(ground::gr_xpos - rapid * l, ground::gr_ypos + p + array_left[l]);
        ofVertex(ground::gr_xpos - rapid * (l + 1), ground::gr_ypos + p + array_left[l + 1]);
        ofVertex(ground::gr_xpos - rapid * (l + 1), ofGetHeight());
        ofVertex(ground::gr_xpos - rapid * l, ofGetHeight());
        ofEndShape();
    }
    
    for (int m = 0; m < ground::draw_flg + 1; m++) {
        ground::copy_array_right[m] = array_right[m];
        ground::copy_array_left[m] = array_left[m];
    }
}

void ground::flag(float w, float h)
{
    if (ar_flag == 1 && ground::gr_flg == 0) {
         ground::gr_xpos = w;
         ground::gr_ypos = h;
         ground::gr_flg = 1;
     }
     
    if (ar_flag == 1 && ground::gr_flg == 1) {
        ground::draw();
        if (ground::draw_flg < 160) {
            ground::draw_flg ++;
        }
        if (ground::gr_count < ofGetWidth()) {
            ground::gr_count += 8;
            ground::groot_count++;
        }
    }
}

void grass::grass0(float x, float y){
	ofBezier(x, y + p, x, y-a/6 + p, x-a/2, y-a/4 + p, x-a/2, y-a/4 + p);
	ofBezier(x, y + p, x, y-a/6 + p, x+a/2, y-a/4 + p, x+a/2, y-a/4 + p);
}

void grass::grass1(float x, float y){
	ofBezier(x, y + p, x, y-a/5 + p, x-a/3*2, y-a/4 + p, x-a/3*2, y-a/4 + p);
	ofBezier(x, y + p, x, y-a/5 + p, x+a/3*2, y-a/4 + p, x+a/3*2, y-a/4 + p);
	ofBezier(x, y + p, x, y-a/3 + p, x-a/4, y-a/5*3 + p, x-a/4, y-a/5*3 + p);
	ofBezier(x, y + p, x, y-a/3 + p, x+a/4, y-a/5*3 + p, x+a/4, y-a/5*3 + p);
}

void grass::grass1_left(float x, float y){
	ofBezier(x, y + p, x, y-a/5 + p, x-a/3*2, y-a/4 + p, x-a/3*2, y-a/4 + p);
	ofBezier(x, y + p, x, y-a/3 + p, x-a/4, y-a/5*3 + p, x-a/4, y-a/5*3 + p);
	ofBezier(x, y + p, x, y-a/3 + p, x+a/4, y-a/5*3 + p, x+a/4, y-a/5*3 + p);
}

void grass::grass1_right(float x, float y){
	ofBezier(x, y + p, x, y-a/5 + p, x+a/3*2, y-a/4 + p, x+a/3*2, y-a/4 + p);
	ofBezier(x, y + p, x, y-a/3 + p, x-a/4, y-a/5*3 + p, x-a/4, y-a/5*3 + p);
	ofBezier(x, y + p, x, y-a/3 + p, x+a/4, y-a/5*3 + p, x+a/4, y-a/5*3 + p);
}

void grass::draw(){
    if (ground::gr_count > 30) {
        grass0(50 + grass::w, grass::h);
        grass1(50 + grass::w, grass::h);
    }
    if (ground::gr_count > 80) {
        grass1_left(-120 + grass::w, grass::h);
        grass1_right(100 + grass::w, grass::h);
    }
    if (ground::gr_count > 160) {
        grass1(-250 + grass::w, grass::h);
        grass0(-250 + grass::w, grass::h);
        grass1_left(-180 + grass::w, grass::h);
        grass1_right(270 + grass::w, grass::h);
    }
    if (ground::gr_count > 220) {
        grass1(-450 + grass::w, grass::h);
        grass0(-450 + grass::w, grass::h);
        grass1_left(-360 + grass::w, grass::h);
        grass1_right(370 + grass::w, grass::h);
    }
    if (ground::gr_count > 270) {
        grass1_left(-560 + grass::w, grass::h);
        grass1_right(540 + grass::w, grass::h);
    }
    if (ground::gr_count > 320) {
        grass1_left(-670 + grass::w, grass::h);
        grass1_right(620 + grass::w, grass::h);
    }
    if (ground::gr_count > 380) {
        grass1_left(-750 + grass::w, grass::h);
        grass1_right(780 + grass::w, grass::h);
    }
    if (ground::gr_count > 450) {
        grass1_left(-910 + grass::w, grass::h);
        grass1_right(890 + grass::w, grass::h);
    }
    if (ground::gr_count > 540) {
        grass1_left(-1100 + grass::w, grass::h);
        grass1_right(1060 + grass::w, grass::h);
    }
    if (ground::gr_count > 620) {
        grass1_left(-1240 + grass::w, grass::h);
        grass1_right(1250 + grass::w, grass::h);
    }
    if (ground::gr_count > 680) {
        grass1_left(-1410 + grass::w, grass::h);
        grass1_right(1370 + grass::w, grass::h);
    }
}

void tree::main(int x, int y, float a, int b, float w, float h){
    float s = 1.2; // scale
    ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetColor(0);
	ofEllipse(x * s + w - 2, y * s + h - 45, a*2 * s, a/3*4 * s);
	ofCircle(x-(a/4 * s) + w, y-(a/4 * s) + h - 80, a/2 * s);
	ofCircle(x+(a/4 * s) + w, y-(a/3 * s) + h - 80, a/5*3 * s);
	ofCircle(x-(a/5*3 * s) + w, y+(a/4 * s) + h - 10, a/2 * s);
	ofCircle(x+(a/5*3 * s) + w, y+(a/3 * s) + h - 10, a/5*3 * s);
	ofCircle(x-(a/5 * s) + w, y+(a/3 * s) + h, a/5*3 * s);
	ofCircle(x+(a/5 * s) + w, y+(a/3 * s) + h, a/5*3 * s);
    ofRect(w, h + 50, 13.5, 85);
    ofSetRectMode(OF_RECTMODE_CORNER);
}

void tree::root(int x, int y , float a, float w, float h){
	int rw = a/8;
    float s = 2; // scale
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(0);
    ofPushMatrix();
    ofTranslate(1.6, -7.5);
	ofTriangle((x-a/6) / s + w, (y+a*2) / s + h, (x+a/10) / s + w, (y+a*2) / s + h, (x+a/20*3) / s + w, (y+a*4) / s + h);
	ofTriangle((x+a/10) / s + w, (y+a*3) / s + h, (x+a/10) / s + w, (y+a*3+rw) / s + h, (x+a/2) / s + w, (y+a*3.5) / s + h);
	ofTriangle((x-a/10) / s + w, (y+a*2) / s + h, (x+a/10) / s + w, (y+a*2) / s + h, (x-a/5) / s + w, (y+a*3.5) / s + h);
	ofTriangle(x / s + w, (y+a*2) / s + h, x / s + w, (y+a*2+rw) / s + h, (x-a*1.5) / s + w, (y+a*2+a/3) / s + h);
	ofTriangle(x / s + w, (y+a*2+a/20) / s + h, x / s + w, (y+a*2+rw+a/20) / s + h, (x+a*1.5) / s + w, (y+a*2+a/3+a/5) / s + h);
	ofTriangle((x+a/2) / s + w, (y+a*2+a/4) / s + h, (x+rw+a/2) / s + w, (y+a*2+a/4) / s + h, (x+a*1.3) / s + w, (y+a*2+a/4*3) / s + h);
	ofTriangle(x / s + w, (y+a*2+a/20-7) / s + h, x / s + w, (y+a*2+rw+a/20) / s + h, (x-a) / s + w, (y+a*2+a+a/10*2) / s + h);
	ofTriangle((x-a/16*3) / s + w, (y+a*2+a/3) / s + h, (x-a/16*3-rw) / s + w, (y+a*2+a/3+rw) / s + h, (x-a) / s + w, (y+a*2+a/5*3) / s + h);
	ofTriangle(x / s + w, (y+a*2+a/5) / s + h, x / s + w, (y+a*2+rw+a/5) / s + h, (x-a/2) / s + w, (y+a*2+a+a/5) / s + h);
	ofTriangle(x / s + w, (y+a*2+a/10*2) / s + h, x / s + w, (y+a*2+rw+a/10*2) / s + h, (x+a/4*3) / s + w, (y+a*2+a+a/10*2) / s + h);
	ofTriangle(x / s + w, (y+a*2+a/10*2) / s + h, x / s + w, (y+a*2+rw+a/10*2) / s + h, (x+a/4*3) / s + w, (y+a*2+a/2) / s + h);
	ofTriangle(x / s + w, (y+a*2+a/5*2) / s + h, x / s + w, (y+a*2+rw+a/5*2) / s + h, (x+a/2) / s + w, (y+a*2+a+a/5*2) / s + h);
	ofTriangle((x+a/20) / s + w, (y+a*2+a/5*3) / s + h, (x+a/20+rw) / s + w, (y+a*2+a/5*3+rw*1.5) / s + h, (x+a/2) / s + w, (y+a*2+a*0.8+a/5) / s + h);
    ofPopMatrix();
    ofSetRectMode(OF_RECTMODE_CORNER);
}

void fan::setup(float w, float h){
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(0);
    ofCircle(w, h - 35, 50);
    ofRect(w, h + 50, 13.5, 85);
    ofSetRectMode(OF_RECTMODE_CORNER);
}

// --------------------------------------------------------------
//  Functions
// --------------------------------------------------------------
void testApp::setup(){
    ofBackground(255, 255, 255);
	ofSetFrameRate(60);
	ofEnableSmoothing();

    cam2.listDevices();
	cam2.setDeviceID(0);
	cam2.setVerbose(true);
	cam2.initGrabber(ofGetWidth(), ofGetHeight());
	color2.allocate(ofGetWidth(), ofGetHeight());
	gray2.allocate(ofGetWidth(), ofGetHeight());

    cap_thresh = 80;

    ARParam  wparam;
	
    int xsize, ysize;

    /* open the video path */
    if( arVideoOpen( vconf ) < 0 ) ofExit();
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) ofExit();
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);
    
    /* set the initial camera parameters */
    if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        ofExit();
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );
    
    if( (patt_id=arLoadPatt(patt_name)) < 0 ) {
        printf("pattern load error !!\n");
        ofExit();
    }
    
    arVideoCapStart();
}

//--------------------------------------------------------------
void testApp::update(){
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    static int      visflag = 0;

	bool isNewFrame2(false);
	cam2.update();
	isNewFrame2 = cam2.isFrameNew();

    if(isNewFrame2){
		color2.setFromPixels(cam2.getPixels(), ofGetWidth(), ofGetHeight());
		gray2 = color2;
	}

    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }
    
    /* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, cog_thresh, &marker_info, &marker_num) < 0 ) {
        ofExit();
    }

    arVideoCapNext();

    /* check for object visibility */
    k = -1;
    for( int j = 0; j < marker_num; j++ ) {
        if( patt_id == marker_info[j].id ) {
            if( k == -1 ) k = j;
            else if( marker_info[k].cf < marker_info[j].cf ) k = j;
            draw_flag = true;
        }
    }
    if( k == -1 ) {
        draw_flag = false;
        do_count = 0;
        test_count = 0;
        ar_flag = 0;
        for (snow::l = 0; snow::l < SNOW; snow::l++) {
            snow::snowxpos_flg[snow::l] = 0;
            snow::snowypos_flg[snow::l] = 0;
            snow::down_count[snow::l] = 0;
            snow::xflow[snow::l] = 0;
            snow::yflow[snow::l] = 0;
        }
        ground::gr_flg = 0;
        ground::draw_flg = 0;
        ground::gr_count = 0;
        ground::groot_count = 0;
        grass::flg = 0;
        return;
    }

    if (visflag == 0) {
        /* get the transformation between the marker and the real camera */
        arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);
    }
    else {
        arGetTransMatCont(&marker_info[k], patt_trans, patt_center, patt_width, patt_trans);
    }
    visflag = 1;
    
    if (k != -1) {
        mk_posx = marker_info[k].pos[0];
        mk_posy = marker_info[k].pos[1];
    }
    
    // hikaku
    if (do_count > 0 && test_count < 15) {
        if ((0 <= (patt_trans[0][3] - pre_patt_trans[0]) && (patt_trans[0][3] - pre_patt_trans[0]) < 3) || (0 >= (patt_trans[0][3] - pre_patt_trans[0]) && (patt_trans[0][3] - pre_patt_trans[0]) > -3)) {
            test_count++;
        }
        else{
            test_count = 0;
            ar_flag = 0;
        }
    }
    if (test_count >= 15) {
        ar_flag = 1;
    }
    
    pre_patt_trans[0] = patt_trans[0][3];
    pre_patt_trans[1] = patt_trans[1][3];
    pre_patt_trans[2] = patt_trans[2][3];
    do_count++;
}

//--------------------------------------------------------------
void testApp::draw(){
    fan fn_draw;
    tree tr_draw;
    ground gr_draw;
    grass gs_draw;
    snow sn_draw;
    
    ofSetColor(255, 255, 255);
	gray2.draw(0, 0);

    rotX = patt_trans[2][1] * 90;
    rotY = patt_trans[0][2] * 90;
    if (patt_trans[1][1] >= -1 && patt_trans[1][1] <= 0) {
        rotZ = patt_trans[0][1] * 90;
    } else {
        if (patt_trans[0][1] > 0) {
            rotZ = (1 * 90) + (patt_trans[1][1] * 90);
        } else if (patt_trans[0][1] < 0) {
            rotZ = -((1 * 90) + (patt_trans[1][1] * 90));
        } else if (patt_trans[0][1] == 0) {
            rotZ = 2 * 90;
        }
    }

    glMatrixMode( GL_MODELVIEW );

    ofPushMatrix();
    if (draw_flag == TRUE) {
        if (ar_flag == 1) {
            ofPushMatrix();
            ofTranslate(mk_posx, mk_posy);
            ofRotateX(rotX);
            ofRotateY(rotY);
            ofRotateZ(rotZ);
            glPushMatrix();
            glScalef((1000 - patt_trans[2][3]) / 500, (1000 - patt_trans[2][3]) / 500, 0); // tree Z position
            glTranslatef(0, (patt_trans[2][3] / 30) - (1000 - patt_trans[2][3]) / 50, 0);  // tree Y position with garound
            ofTranslate(-mk_posx, -mk_posy);
            tr_draw.main(0, 0, -50, 100, mk_posx, mk_posy);
            if (ground::groot_count > 15) {
                tr_draw.root(0, 0, 100, mk_posx, mk_posy);
            }
            glPopMatrix();
            ofPopMatrix();
            gr_draw.flag(mk_posx, mk_posy);
            if (grass::flg == 0) {
                grass::w = mk_posx;
                grass::h = mk_posy;
                grass::flg = 1;
            }
            gs_draw.draw();
            sn_draw.draw();
        }
        if (ar_flag == 0) {
            ofPushMatrix();
            ofTranslate(mk_posx, mk_posy);
            ofRotateX(rotX);
            ofRotateY(rotY);
            ofRotateZ(rotZ);
            glPushMatrix();
            glScalef((1000 - patt_trans[2][3]) / 500, (1000 - patt_trans[2][3]) / 500, 0); // tree Z position
            glTranslatef(0, (patt_trans[2][3] / 30) - (1000 - patt_trans[2][3]) / 50, 0);  // tree Y position with garound
            ofTranslate(-mk_posx, -mk_posy);
            fn_draw.setup(mk_posx, mk_posy);
            glPopMatrix();
            ofPopMatrix();
        }
    }
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key){
            
        case 0x1B:						// Quit.
            arVideoCapStop();
            arVideoClose();
            cam2.close();
			ofExit();
			break;

		case OF_KEY_UP:
			cog_thresh ++;
			if(cog_thresh > 255) cog_thresh = 255;
			break;
            
		case OF_KEY_DOWN:
			cog_thresh --;
			if(cog_thresh < 0) cog_thresh = 0;
			break;
			
			
		case OF_KEY_RIGHT:
			cap_thresh ++;
			if(cap_thresh > 255) cap_thresh = 255;
			break;
			
		case OF_KEY_LEFT:
			cap_thresh --;
			if(cap_thresh < 0) cap_thresh = 0;
			break;
			
		case 'x':
			cam2.videoSettings();
			break;
			
		case ' ':
			if(ofGetWindowMode() == OF_WINDOW){
				ofSetFullscreen(true);
			}else{
				ofSetFullscreen(false);
			}
			break;
            
        default:
            break;
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
