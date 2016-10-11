#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "../particle.h"

enum mode {
	SEA_CALM,
	SEA_STORM,
	CYCLOP,
	MERMAID_CALM,
	MERMAID_ATTACK,
	START,
	END
};

enum particles_state {
	FADEOUT,
	FADEIN,
	SHOWING,
	HIDING,
};

class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxKinect kinect;
		ofEasyCam easyCam;
		int angle;

		ofImage cyclop_eye;
		vector<particle> particles;
		mode mode;
		particles_state p_state;
		float alpha;
		float particle_radius;
		float usr_radius;
		float img_size;
		ofSoundPlayer  vocal;
		float transparency_rate;

		float start_time;
		float timer;

		ofTrueTypeFont	font;
		float fontSize;
		float fontSpacing;
		vector<string> paragraph;
		
		ofPoint mass;
		ofImage img;
};
