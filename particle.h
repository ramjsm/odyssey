#pragma once
#include "ofMain.h"

class particle {
public:
	// Constructor
	particle(ofPoint _init, float _radius);

	// Methods
	void moveTo(ofPoint _dest);
	void traslateParticlesGroup(ofPoint _dest, float _center);
	void moveToInit();
	void explode();
	void moveToDir();
	void repel(float _dist, ofVec2f _v);
	void moveLikeWave();
	void moveLikeWaveStorm();
	void repultion(float _dist, float _usr_radius, ofVec2f _v);
	void attackTo(ofPoint _dest);
	void draw();


	// Properties
	ofVec2f init_position;
	ofVec2f position;
	ofVec2f dir;
	float radius;
	float offset_init;
	float offset;
	float force;
	float speed;
	float offset_storm;

};
