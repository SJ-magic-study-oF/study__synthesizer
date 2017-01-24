/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxNetwork.h"
#include "synthe.h"

#include "common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		WIDTH = 1280,
		HEIGHT = 720,
		
		N = 256,
	};
	enum{
		SIZE_UPD = 100000,
	};
	
	
	/****************************************
	****************************************/
	/********************
	********************/
	SYNTHE Synthe;
	
	ofxUDPManager udpConnection;
	
	/********************
	********************/
	bool b_1stMessage;
	float spectrum[ N ];
	
	bool f_fullScreen;
	
	/********************
	********************/
	ofVideoPlayer video;
	
	bool b_disp_video;
	
	/********************
	********************/
	ofShader shader;
	ofVideoGrabber VideoCam;
	
	ofxPanel gui;
	ofxToggle b_showVideoCam;
	ofxFloatSlider VideoCam_alpha;
	ofxIntSlider ksectors;
	ofxToggle b_SelfRotation;
	float k_angle;
	
	bool b_showGui;
	
	


	
public:
	/****************************************
	****************************************/
	ofApp(int mode);
    void exit();

	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
		
};
