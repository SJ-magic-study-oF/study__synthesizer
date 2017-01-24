/************************************************************
************************************************************/
#include "synthe.h"


/************************************************************
************************************************************/

/******************************
******************************/
void SYNTHE::exit() {
	// gui.saveToFile( "settings_exit.xml" );
}

/******************************
******************************/
void SYNTHE::setup(){
	/********************
	********************/
	gui.setup( "Parameters", "settings.xml" );
	
	gui.add( LineWidth.setup( "LineWidth", 1, 1, 10 ) );
	
	gui.add(MusicSync.setup("MusicSync", false));
	gui.add(b_SelfRotation.setup("Self Rotaion", false));
	
	gui.add( countX.setup( "countX", 0, 0, 200 ) );
	gui.add( stepX.setup( "stepX", 200, 0, 300 ) );
	gui.add( twistX.setup( "twistX", 0, -45, 45 ) );
	
	gui.add( countY.setup( "countY", 0, 0, 100) );
	gui.add( stepY.setup( "stepY", 20, 0, 300 ) );
	gui.add( twistY.setup( "twistY", 0, -30, 30 ) );
	gui.add( pinchY.setup( "pinchY", 0, 0, 1 ) );
	
	globalGroup.setup( "Global" );
	globalGroup.add( g_Scale.setup( "g_Scale", 0.7, 0.0, 1 ) );
	globalGroup.add( g_Rotate.setup( "g_Rotate", 0, -180, 180 ) );
	// globalGroup.add( Background.setup( "Background", 0, 0, 255 ) );
	gui.add( &globalGroup );
	
	/* */
	primGroup.setup( "Primitive" );
	primGroup.add( primitive_shiftY.setup( "shiftY", 0.0, -1000.0, 1000.0 ) );
	primGroup.add( primitive_rotate.setup( "rotate", 0.0, -180.0, 180.0 ) );
	
	primGroup.add( b_PrimitiveSize_by_x.setup( "size by x", true ) );
	primGroup.add( primitive_size.setup(	"size",
											ofVec2f(6,6),
											ofVec2f(0,0),
											ofVec2f(20,20) ) );
	
	//color.setDefaultBackgroundColor( ofColor::gray );
	primGroup.add( b_DynamicColor.setup( "b_DynamicColor", false ) );
	primGroup.add( color.setup(	"color",
								ofColor(50, 50, 255, 255),
								ofColor(0,0,0,0),
								ofColor::white ) );
	primGroup.add( filled.setup( "filled", false ) );
	primGroup.add( type.setup( "type", false ) );
	gui.add( &primGroup );

	// gui.loadFromFile( "settings.xml" );
	showGui = true;
	
	/********************
	********************/
	ofVec3f point0(0, 0);
	for(int i = 0; i < NUM_VERTS; i++){
		vbo_Verts[i].set(point0);
		vbo_Color[i].set(1.0, 0.0, 0.0, 1.0);
	}
	
	Vbo.setVertexData(vbo_Verts, NUM_VERTS, GL_DYNAMIC_DRAW);
	Vbo.setColorData(vbo_Color, NUM_VERTS, GL_DYNAMIC_DRAW);
	
	/********************
	********************/
	id_pattern = -1;
}

/******************************
******************************/
void SYNTHE::update(float* spectrum, int N, int R, int G, int B){

	if(b_DynamicColor){
		ofColor Target = color; // a のみ、Gui情報から反映.
		Target.r = R;
		Target.g = G;
		Target.b = B;
		
		color = Target;
	}
	
	if(MusicSync)		changeParam_MusicSync(spectrum, N);
	if(b_SelfRotation)	SelfRotaion();
}

/******************************
******************************/
void SYNTHE::changeParam_MusicSync(float* spectrum, int N)
{
	float Lev = spectrum[1];
	
	switch(id_pattern){
		case -1:
			countX = ofMap(Lev, 0, 2.0, 0, 200, true);
			break;
			
		case 0:
		case 1:
			stepX = ofMap(Lev, 0, 2.0, 50, 200, true);
			break;
			
		case 2:
		{
			stepX = ofMap(Lev, 0, 2.0, 50, 200, true);
		}
			break;
			
		case 3:
		case 4:
		{
			twistX = ofMap(Lev, 0, 2.0, 0, 45, true);
		}
			break;
			
		case 5:
		{
			stepX = ofMap(Lev, 0, 2.0, 60, 300, true);
		}
			break;
			
		case 6:
		{
			stepX = ofMap(Lev, 0, 2.0, 0, 300, true);
			
			ofColor temp = color;
			temp.a = ofMap(Lev, 0, 2.0, 0, 120, true);
			color = temp;
		}
			break;
			
		case 7:
		{
			stepX = ofMap(Lev, 0, 2.0, 0, 200, true);
			// stepY = ofMap(Lev, 0, 2.0, 0, 150, true);
			
			ofColor temp = color;
			temp.a = ofMap(Lev, 0, 2.0, 50, 180, true);
			color = temp;
		}
			break;
			
	}
}

/******************************
******************************/
void SYNTHE::SelfRotaion()
{
	float now = ofGetElapsedTimef();
	
	switch(id_pattern){
		case 0:
		case 1:
		{
			const float deg_per_sec = 360.0 / 30.0;
			g_Rotate = now * deg_per_sec;
			
			const float T = 80.0;
			twistX = 45 * sin(2 * PI / T * now);
		}
			break;
			
		case 2:
		{
			const float deg_per_sec = 360.0 / 30.0;
			g_Rotate = now * deg_per_sec;
			
			const float T = 20.0;
			twistY = 5 * sin(2 * PI / T * now);

			twistX = 13 * sin(2 * PI / T * now);
		}
			break;
			
		case 3:
		case 4:
		{
			const float T = 30.0;
			primitive_rotate = 180 * sin(2 * PI / T * now);
		}
			break;
			
		case 5:
		{
			const float deg_per_sec = 360.0 / 30.0;
			g_Rotate = now * deg_per_sec;
			
			const float T = 20.0;
			stepY = 150 * sin(2 * PI / T * now) + 150;
			
			primitive_shiftY = 700 * sin(2 * PI / T * now);
		}
			break;
			
		case 6:
		{
			const float deg_per_sec = 360.0 / 30.0;
			g_Rotate = now * deg_per_sec;

			const float T = 10.0;
			stepY = 70 * sin(2 * PI / T * now) + 70;
		}
			break;
			
		case 7:
		{
			const float deg_per_sec = 360.0 / 60.0;
			g_Rotate = now * deg_per_sec;
			
			const float T = 30.0;
			twistY = 4 * sin(2 * PI / T * now);
		}
			break;
	}
}

/******************************
description
	x方向への広がりを描画

	what this function do is...
		for (int i=-countX; i<=countX; i++) {
			ofPushMatrix();
			ofTranslate( i * stepX, 0 );
			ofRotate( i * twistX );
			
			ofTranslate( 0, primitive_shiftY );
			ofRotate( primitive_rotate );
			
			if(b_PrimitiveSize_by_x)	ofScale( primitive_size->x, primitive_size->x );
			else						ofScale( primitive_size->x, primitive_size->y );
			
			ofTriangle( 0, 0, -50, 100, 50, 100 );
			// ofRect( -50, -50, 100, 100 );
			
			ofPopMatrix();
		}
	
	But source code above is too heavy for ordinary CPU.
	So here, I use ofVbo.
******************************/
void SYNTHE::stripePattern()
{
	ofSetColor( color );
	ofSetLineWidth( LineWidth );
	if ( filled ) ofFill(); else ofNoFill();
	
	for (int i=-countX; i<=countX; i++) {
		/********************
		********************/
		AXIS axis;
		
		axis.translate(i * stepX, 0, 0);
		axis.rotate(i * twistX, ofVec3f(0, 0, 1));
		
		axis.translate(0, primitive_shiftY, 0);
		axis.rotate(primitive_rotate, ofVec3f(0, 0, 1));
		
		if(b_PrimitiveSize_by_x)	axis.scale(primitive_size->x, primitive_size->x, 1);
		else						axis.scale(primitive_size->x, primitive_size->y, 1);
		
		/********************
		********************/
		ofVec3f target;
		
		if(type){ // Rect
			axis.get(ofVec3f(-50, -50, 0), target);
			vbo_Verts[(i + countX) * 4 + 0].set(target);

			axis.get(ofVec3f(-50, 50, 0), target);
			vbo_Verts[(i + countX) * 4 + 1].set(target);

			axis.get(ofVec3f(50, 50, 0), target);
			vbo_Verts[(i + countX) * 4 + 2].set(target);

			axis.get(ofVec3f(50, -50, 0), target);
			vbo_Verts[(i + countX) * 4 + 3].set(target);
			
		}else{ // Triangle
			axis.get(ofVec3f(0, 0, 0), target);
			vbo_Verts[(i + countX) * 3 + 0].set(target);

			axis.get(ofVec3f(-50, 100, 0), target);
			vbo_Verts[(i + countX) * 3 + 1].set(target);

			axis.get(ofVec3f(50, 100, 0), target);
			vbo_Verts[(i + countX) * 3 + 2].set(target);
		}
	}
	
	if(type)	Vbo.updateVertexData(vbo_Verts, (countX * 2 + 1) * 4);
	else		Vbo.updateVertexData(vbo_Verts, (countX * 2 + 1) * 3);
	
	ofColor temp = color;
	for(int i = 0; i < NUM_VERTS; i++){
		vbo_Color[i].set( float(temp.r)/255, float(temp.g)/255, float(temp.b)/255, float(temp.a)/255);
	}
	Vbo.updateColorData(vbo_Color, NUM_VERTS);
	
	if(type)	Vbo.draw(GL_QUADS, 0, (countX * 2 + 1) * 4);
	else		Vbo.draw(GL_TRIANGLES, 0, (countX * 2 + 1) * 3);
}

/******************************
******************************/
void SYNTHE::matrixPattern()
{
	for (int y=-countY; y<=countY; y++) {
		ofPushMatrix();

		if ( countY > 0 ) { // if countY is set.
			float scl = ofMap( y, -countY, countY, 1-pinchY, 1 );
			ofScale( scl, scl );
		}
		ofTranslate( 0, y * stepY );
		ofRotate( y * twistY );
		stripePattern();
		
		ofPopMatrix();
	}
}


/******************************
******************************/
void SYNTHE::draw()
{
	// ofBackground( Background );
	
	ofPushStyle();
	
	ofPushMatrix();
	ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
	float Scl = pow( g_Scale, 4.0f );
	ofScale( Scl, Scl );
	ofRotate( g_Rotate );

	matrixPattern();
	
	ofPopMatrix();
	
	ofPopStyle();
	
	
	if ( showGui ) gui.draw();
}

/******************************
******************************/
void SYNTHE::keyPressed(int key)
{
	switch(key){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		{
			id_pattern = key - '0';
			
			enum{
				BUF_SIZE = 500,
			};
			char FileName[BUF_SIZE];
			sprintf(FileName, "%d.xml", id_pattern);
			gui.loadFromFile(FileName);
			
			if(BootMode == BOOTMODE_PERFORMANCE){
				MusicSync = true;
				b_SelfRotation = true;
				b_DynamicColor = true;
			}
		}
			break;
		
		case 'h':
			showGui = !showGui;
			break;
			
		case 'l':
		{
			ofFileDialogResult res;
			res = ofSystemLoadDialog( "Loading Preset" );
			if ( res.bSuccess ) gui.loadFromFile( res.filePath );
		}
			break;
			
		case 'r':
			id_pattern = -1;
			gui.loadFromFile( "reset.xml" );
			break;
			
		case 's':
		{
			ofFileDialogResult res;
			res = ofSystemSaveDialog( "preset.xml", "Saving Preset");
			if ( res.bSuccess ) gui.saveToFile( res.filePath );
		}
			break;
	}

}

