/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxGui.h"

#include "common.h"

/************************************************************
************************************************************/

/**************************************************
description
	基底ベクトルを定義し、これに対し、
	translate, rotat, scale
	のmethodを準備。
	
	「変換後の基底ベクトル」を「正規直交ベクトル空間で表現した値」を得る( method = get() )ことが目的。
**************************************************/
class AXIS{
private:
	/********************
	Default:
		正規直交 基底Vector
		
	計算結果:
		「変換後の基底ベクトル」を「正規直交空間で表現した値」が格納される。
	********************/
	ofVec3f e_x;
	ofVec3f e_y;
	ofVec3f e_z;
	
	ofVec3f center;
	
public:
	AXIS() : e_x(1, 0, 0), e_y(0, 1, 0), e_z(0, 0, 1), center(0, 0, 0){
	}
	
	void Reset()
	{
		e_x = ofVec3f(1, 0, 0);
		e_y = ofVec3f(0, 1, 0);
		e_z = ofVec3f(0, 0, 1);
		
		center = ofVec3f(0, 0, 0);
	}
	
	
	void translate(float x, float y, float z)
	{
		center += x * e_x;
		center += y * e_y;
		center += z * e_z;
	}
	
	/******************************
	URL
		Rotate quad made in geometry shader
			http://stackoverflow.com/questions/28124548/rotate-quad-made-in-geometry-shader
			
			shader上での実装も記述してあり、大変参考になる.
	******************************/
	void rotate(float angle_deg, ofVec3f _n)
	{
		/********************
		ofRotateと方向合わせる.
		********************/
		float angle_rad = -angle_deg * 3.1415 / 180.0;
		
		/********************
		_n の周りにangle_degの回転を施す。
		
		-n は、変換後の基底ベクトルでの表現。
		計算は全て、正規直交ベクトルを用いて行う。
		e_xyzは、すでにmatrix変換されているかもしれないので、まずは
		_n を正規直交ベクトル表現で表すと・・・
		********************/
		ofVec3f n(0, 0, 0);
		n += e_x * _n.x;
		n += e_y * _n.y;
		n += e_z * _n.z;
		
		
		/********************
		n(_nを正規直交ベクトル空間で表現)を中心とし、angle_rad 回転させると・・・
		********************/
		ofVec3f q;
		q.x =	  e_x.x * (n.x * n.x * (1.0 - cos(angle_rad)) + cos(angle_rad))
				+ e_x.y * (n.x * n.y * (1.0 - cos(angle_rad)) + n.z * sin(angle_rad))
				+ e_x.z * (n.x * n.z * (1.0 - cos(angle_rad)) - n.y * sin(angle_rad));
		
		q.y =   e_x.x * (n.y * n.x * (1.0 - cos(angle_rad)) - n.z * sin(angle_rad))
				+ e_x.y * (n.y * n.y * (1.0 - cos(angle_rad)) + cos(angle_rad))
				+ e_x.z * (n.y * n.z * (1.0 - cos(angle_rad)) + n.x * sin(angle_rad));
		
		q.z =   e_x.x * (n.z * n.x * (1.0 - cos(angle_rad)) + n.y * sin(angle_rad))
				+ e_x.y * (n.z * n.y * (1.0 - cos(angle_rad)) - n.x * sin(angle_rad))
				+ e_x.z * (n.z * n.z * (1.0 - cos(angle_rad)) + cos(angle_rad));
				
		e_x = q;
				
		/********************
		********************/
		q.x =	  e_y.x * (n.x * n.x * (1.0 - cos(angle_rad)) + cos(angle_rad))
				+ e_y.y * (n.x * n.y * (1.0 - cos(angle_rad)) + n.z * sin(angle_rad))
				+ e_y.z * (n.x * n.z * (1.0 - cos(angle_rad)) - n.y * sin(angle_rad));
		
		q.y =   e_y.x * (n.y * n.x * (1.0 - cos(angle_rad)) - n.z * sin(angle_rad))
				+ e_y.y * (n.y * n.y * (1.0 - cos(angle_rad)) + cos(angle_rad))
				+ e_y.z * (n.y * n.z * (1.0 - cos(angle_rad)) + n.x * sin(angle_rad));
		
		q.z =   e_y.x * (n.z * n.x * (1.0 - cos(angle_rad)) + n.y * sin(angle_rad))
				+ e_y.y * (n.z * n.y * (1.0 - cos(angle_rad)) - n.x * sin(angle_rad))
				+ e_y.z * (n.z * n.z * (1.0 - cos(angle_rad)) + cos(angle_rad));
				
		e_y = q;

		/********************
		********************/
		q.x =	  e_z.x * (n.x * n.x * (1.0 - cos(angle_rad)) + cos(angle_rad))
				+ e_z.y * (n.x * n.y * (1.0 - cos(angle_rad)) + n.z * sin(angle_rad))
				+ e_z.z * (n.x * n.z * (1.0 - cos(angle_rad)) - n.y * sin(angle_rad));
		
		q.y =   e_z.x * (n.y * n.x * (1.0 - cos(angle_rad)) - n.z * sin(angle_rad))
				+ e_z.y * (n.y * n.y * (1.0 - cos(angle_rad)) + cos(angle_rad))
				+ e_z.z * (n.y * n.z * (1.0 - cos(angle_rad)) + n.x * sin(angle_rad));
		
		q.z =   e_z.x * (n.z * n.x * (1.0 - cos(angle_rad)) + n.y * sin(angle_rad))
				+ e_z.y * (n.z * n.y * (1.0 - cos(angle_rad)) - n.x * sin(angle_rad))
				+ e_z.z * (n.z * n.z * (1.0 - cos(angle_rad)) + cos(angle_rad));
				
		e_z = q;
	}

	void scale(float x, float y, float z)
	{
		e_x *= x;
		e_y *= y;
		e_z *= z;
	}
	
	/******************************
	description
	
	param
		point
			変換後空間でのベクトル値.

		target
			return.
			正規直交空間上での座標.
	******************************/
	void get(ofVec3f point, ofVec3f& target)
	{
		target = center;
		
		target += point.x * e_x;
		target += point.y * e_y;
		target += point.z * e_z;
	}

};

/**************************************************
**************************************************/
class SYNTHE_BASE{
private:
public:
	virtual void setup() = 0;
	virtual void update(float* spectrum, int N, int R, int G, int B) = 0;
	virtual void draw() = 0;
	virtual void keyPressed(int key) = 0;
};

/**************************************************
**************************************************/
class SYNTHE : public SYNTHE_BASE{
private:
	/****************************************
	enum
	****************************************/
	
	
	/****************************************
	param
	****************************************/
	/********************
	********************/
	int id_pattern;
	float t_StartPattern;
	
	/********************
	********************/
    ofxPanel gui;
	ofxFloatSlider LineWidth;
    ofxIntSlider countX;
    ofxFloatSlider stepX;
    ofxFloatSlider twistX;
    
    ofxIntSlider countY;
    ofxFloatSlider stepY, twistY, pinchY;
	
    ofxGuiGroup globalGroup;
    ofxFloatSlider g_Scale;
    ofxFloatSlider g_Rotate;
    // ofxFloatSlider Background;
    
    ofxGuiGroup primGroup;
    ofxFloatSlider primitive_shiftY, primitive_rotate;
    ofxVec2Slider primitive_size;
	ofxToggle b_PrimitiveSize_by_x;
    ofxColorSlider color;
    ofxToggle filled, type;
	
	ofxToggle MusicSync;
	ofxToggle b_SelfRotation;
	ofxToggle b_DynamicColor;
    
    bool showGui;
	
	/****************************************
	****************************************/
	enum{
		/********************
		when countX = 200
		(countX * 2 + 1) * 4(頂点) = 1604.
		
		4000 pointあれば、十分。
		実際に更新するpoint数は、
			Vbo.updateColorData()
		にて指定。
		********************/
		NUM_VERTS = 4000,
	};

	ofVbo Vbo;
	ofVec3f vbo_Verts[NUM_VERTS];
	ofFloatColor vbo_Color[NUM_VERTS];

	/****************************************
	****************************************/
	void stripePattern();
    void matrixPattern();
	
	void changeParam_MusicSync(float* spectrum, int N);
	void SelfRotaion();
	
public:
	/****************************************
	****************************************/
    void exit();

	
	void setup();
	void update(float* spectrum, int N, int R, int G, int B);
	void draw();

	void keyPressed(int key);

};


