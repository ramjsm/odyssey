#include "particle.h"

particle::particle(ofPoint _init, float _radius) {
	
	radius = _radius;
	offset = ofRandom(1);
	offset_storm = ofRandom(1,4);
	init_position = _init;
	position = _init;
	force = 0.0;
	speed = 0.03;
	dir = ofPoint(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()), 0) - position;
}

void particle::moveTo(ofPoint _dest) {
	position += (_dest - position) * speed;

}

void particle::moveToInit() {
	position += (init_position - position) * speed;
}

void particle::explode() {
	force = ofRandom(0.01,0.02);
}

void particle::repultion(float _dist, float _usr_radius, ofVec2f _v) {
	float pct = _dist / _usr_radius;
	pct *= pct;
	pct = 1.0 - pct;
	_v /= _dist;

	position.x += _v.x * pct * 2;
	position.y += _v.y * pct * 2;
}

void particle::moveLikeWave() {

	position.y += offset*cos(ofGetElapsedTimef());
	position.x += offset*cos(ofGetElapsedTimef());

}


void particle::moveLikeWaveStorm() {

	position.y += offset_storm*(cos(ofGetElapsedTimef()) + ofSignedNoise(position.y));
	position.x += offset_storm*(cos(ofGetElapsedTimef()) + ofSignedNoise(position.x));

}

void particle::moveToDir() {
	position += (dir * speed * 10) + force;
}

void particle::repel(float _dist, ofVec2f _v) {
	dir *= -1;
	ofVec2f u =  position - _v;
	u.normalize();
	position += u*_dist * speed;
	force = ofRandom(0.1);

}

void particle::attackTo(ofPoint _dest) {
	position += (_dest - position) * speed * 10;

}

void particle::traslateParticlesGroup(ofPoint _dest, float _center) {
	
	ofVec2f u = (_dest - _center) + init_position;
	position += (u - position )* speed;

}

void particle::draw() {
	ofDrawCircle(position.x, position.y, radius);
}