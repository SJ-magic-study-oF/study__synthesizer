/************************************************************
************************************************************/
#include "ofApp.h"


/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int mode)
{
	BootMode = BOOTMODE(mode);
}

//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle( "Video synth" );
	ofSetWindowShape( WIDTH, HEIGHT );
	
	if(BootMode == BOOTMODE_TEST)	ofSetFrameRate( 60 );
	else							ofSetFrameRate( 30 );

	ofSetVerticalSync(true);
	ofBackground( ofColor::white );
	ofSetEscapeQuitsApp(false);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	/********************
	********************/
	Synthe.setup();
	
	/********************
	********************/
	udpConnection.Create();
	udpConnection.Bind(12345);
	udpConnection.SetNonBlocking(true);
	
	b_1stMessage = true;
	
	/********************
	********************/
	video.loadMovie("movie.mov");
	video.play();
	b_disp_video = false;
	
	/********************
	********************/
	gui.setup( "main", "main.xml" );
	gui.add(b_showVideoCam.setup("b_showVideoCam", false));
	gui.add(b_SelfRotation.setup("b_SelfRot", false));
	gui.add( VideoCam_alpha.setup( "alpha", 0.635, 0.0, 1.0 ) );
	gui.add( ksectors.setup( "ksectors", 0, 0, 10 ) );

	
	k_angle = 0;
	
	b_showGui = true;
	
	/********************
	********************/
	shader.load( "kaleido" );
	VideoCam.initGrabber(WIDTH, HEIGHT);
	
	/********************
	********************/
	f_fullScreen = false;
}

//--------------------------------------------------------------
void ofApp::exit() {
	Synthe.exit();
	
	video.close(); // Calls the closeMovie() function, which closes the movie file and de-allocates resources.
	
	VideoCam.close();
}

//--------------------------------------------------------------
void ofApp::update(){
	/********************
	********************/
	static int R, G, B;
	
	char udpMessage[SIZE_UPD];
	udpConnection.Receive(udpMessage,SIZE_UPD);
	string message=udpMessage;
	if(message!=""){
		vector<string> str = ofSplitString(message, "[/p]");
		
		if(str.size() == 2){
			/********************
			********************/
			vector<string> strColors = ofSplitString(str[0],"|");
			if(strColors.size() == 3){
				R = int(atof(strColors[0].c_str()));
				G = int(atof(strColors[1].c_str()));
				B = int(atof(strColors[2].c_str()));
			}
			
			/********************
			********************/
			vector<string> strValues = ofSplitString(str[1],"|");
			
			if(b_1stMessage){
				printf("Message Size = %d\n", strValues.size() - 1);
				b_1stMessage = false;
			}
			
			for(unsigned int i=0;i<strValues.size() - 1;i++){ // "0" is added at last value.
				if(i < N)	spectrum[i] = atof(strValues[i].c_str());
			}
			
		}
		
		while(message != ""){
			udpConnection.Receive(udpMessage,SIZE_UPD);
			message = udpMessage;
		}
	}
	
	/********************
	********************/
	Synthe.update(spectrum, N, R, G, B);
	
	/********************
	********************/
	video.update();
	VideoCam.update();
	
	static float LastInt_sec = 0;
	float now = ofGetElapsedTimef();
	float dt = ofClamp(now - LastInt_sec, 0, 0.1);
	if(b_SelfRotation){
		const float deg_per_sec = 10;
		k_angle += deg_per_sec * dt;
		if(360 <= k_angle) k_angle -= 360;
	}
	LastInt_sec = now;
}

//--------------------------------------------------------------
void ofApp::draw(){
	/********************
	********************/
	ofBackground( 0 );
	
	/********************
	********************/
	if(b_disp_video){
		if( video.isPlaying() ){
			ofSetColor( 255 );
			video.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
	}
	
	/********************
	********************/
	ofSetColor( 255 );
	
	if(b_showVideoCam){
		shader.begin();
		
		/********************
		VideoCam.draw(0, 0, ofGetWidth(), ofGetHeight());
		において、shaderに渡されるparameterは、
			VideoCam.initGrabber(WIDTH, HEIGHT);
		で初期化した各サイズ。
		shader通過後に、stretchされるようだ。
		
		なので、現状の画面サイズ(ofGetWidth())でなく、初期化時のサイズ(WIDTH, HEIGHT)をshaderに渡さないと、ずれる。
		
		windowResized()でVideoCam.setup(w, h);とすると上のcodeで動作するが、window resizeの度に動作が止まる。
		********************/
		/*
		shader.setUniform2f( "screenCenter", 0.5*ofGetWidth(), 0.5*ofGetHeight() );
		shader.setUniform2f( "kcenter", 0.5*ofGetWidth(), 0.5*ofGetHeight() );
		/*/
		shader.setUniform2f( "screenCenter", 0.5*WIDTH, 0.5*HEIGHT );
		shader.setUniform2f( "kcenter", 0.5*WIDTH, 0.5*HEIGHT );
		//*/
		
		shader.setUniform1i( "ksectors", ksectors );
		shader.setUniform1f( "kangleRad", ofDegToRad(k_angle) );
		shader.setUniform1f( "alpha", VideoCam_alpha );
		VideoCam.draw(0, 0, ofGetWidth(), ofGetHeight());
		
		shader.end();
	}
	
	/********************
	********************/
	Synthe.draw();
	
	
	/********************
	********************/
	if(b_showGui)	gui.draw();

	printf("%5.1f\r", ofGetFrameRate());
	fflush(stdout);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case 'h':
			b_showGui = !b_showGui;
			break;
			
		case 'f':
			f_fullScreen = !f_fullScreen;
			ofSetFullscreen(f_fullScreen);
			if(!f_fullScreen){
				ofSetWindowShape( WIDTH, HEIGHT );
			}
			break;
			
		case 'v':
			b_disp_video = !b_disp_video;
			printf("\nb_disp_video = %d\n", b_disp_video);
			break;
			
		case OF_KEY_RETURN:
			ofSaveScreen( "screenshot.png" );
			break;
			
		default:
			break;
	}
	
	Synthe.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
