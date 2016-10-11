#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);

	// enable depth->video image calibration
	kinect.setRegistration(true);

	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)

	kinect.open();		// opens first available kinect
						//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
						//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #

						// print the intrinsic IR sensor values
	if (kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}

	ofSetFrameRate(60);

	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);

	p_state = HIDING;
	transparency_rate = 1;
	particle_radius = 2.0;
	alpha = 0.0;
	usr_radius = 400;
	img_size = 400;

	fontSize = 40;
	fontSpacing = 1.035;

	font.load("fonts/All rights reserved.ttf", fontSize, true, true);
	font.setLineHeight(fontSize);
	font.setLetterSpacing(fontSpacing);



	start_time = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();

	timer = ofGetElapsedTimef();
	timer -= start_time;

	if (timer < 30 && p_state == HIDING) {
		p_state = FADEIN;
		mode = START;

		paragraph.push_back("As you set out for Ithaka");
		paragraph.push_back("hope the voyage is a long one,");
		paragraph.push_back("full of adventure, full of discovery.");
		paragraph.push_back("Laistrygonians and Cyclops,");
		paragraph.push_back("angry Poseidon - don’t be afraid of them:");
		paragraph.push_back("you’ll never find things like that on your way");
		paragraph.push_back("as long as you keep your thoughts raised high,");
		paragraph.push_back("as long as a rare excitement");
		paragraph.push_back("stirs your spirit and your body.");
	}
	else if (timer > 30 && p_state == SHOWING && mode == START) {
		p_state = FADEOUT;
	}
	else if (timer > 30 && p_state == HIDING && mode == START) {
		mode = SEA_CALM;
		p_state = FADEIN;

		vocal.load("sounds/sea.wav");
		vocal.setVolume(0.75f);

		for (int i = 0; i < ofGetWindowHeight(); i += 20) {
			for (int j = 0; j < ofGetWindowWidth(); j += 20) {
				particle p = particle(ofPoint(j, i), ofRandom(particle_radius));
				particles.push_back(p);
			}
		}
		vocal.play();
	}
	else if (timer > 75 && p_state == SHOWING && mode == SEA_CALM) {
		p_state = FADEOUT;
	}
	else if (timer > 75 && p_state == HIDING && mode == SEA_CALM) {
		particles.clear();
		mode = CYCLOP;
		p_state = FADEIN;

		bool succ = true;
		succ = cyclop_eye.load("images/eye3.jpg");
		if (!succ) {
			cerr << "loading image failed ...\n";
		}

		cyclop_eye.resize(img_size, img_size);

		vocal.load("sounds/cyclop.wav");
		vocal.setVolume(0.75f);

		float intensityThreshold = 50.0;
		int w = cyclop_eye.getWidth();
		int h = cyclop_eye.getHeight();
		for (int x = 0; x < w; x += 5) {
			for (int y = 0; y < h; y += 5) {

				ofColor c = cyclop_eye.getColor(x, y);
				float intensity = c.getLightness();
				if (intensity >= intensityThreshold) {
					particles.push_back(particle(ofPoint(x, y), ofRandom(1, particle_radius)));
				}
			}
		}
		vocal.play();
	}
	else if (timer > 120 && p_state == SHOWING && mode == CYCLOP) {
		p_state = FADEOUT;
	}
	else if (timer > 120 && p_state == HIDING && mode == CYCLOP) {
		mode = SEA_STORM;
		p_state = FADEIN;
		particles.clear();

		vocal.load("sounds/storm.wav");
		vocal.setVolume(0.75f);

		for (int i = 0; i < ofGetWindowHeight(); i += 20) {
			for (int j = 0; j < ofGetWindowWidth(); j += 20) {
				particle p = particle(ofPoint(j, i), ofRandom(particle_radius));
				particles.push_back(p);
			}
		}
		vocal.play();
	}
	else if (timer > 165 && p_state == SHOWING && mode == SEA_STORM) {
		p_state = FADEOUT;
	}
	else if (timer > 165 && p_state == HIDING && mode == SEA_STORM) {
		mode = MERMAID_CALM;
		p_state = FADEIN;


		float wave_size = 200;
		float angle = 360 / wave_size;

		vocal.load("sounds/mermaids.wav");
		vocal.setVolume(0.75f);
		for (int j = 0; j < 20; j++) {
			ofPoint random = ofPoint(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
			for (int i = 0; i < wave_size; i++) {
				particle p = particle(random, ofRandom(particle_radius));
				p.dir = ofPoint(cos(i*angle), sin(i*angle));
				particles.push_back(p);
			}
		}
		vocal.play();
	}
	else if (timer > 210 && p_state == SHOWING && mode == MERMAID_CALM) {
		mode = MERMAID_ATTACK;
		vocal.load("sounds/mermaids_attack.wav");
		vocal.setVolume(0.75f);
		vocal.play();
	}
	else if (timer > 255 && p_state == SHOWING && mode == MERMAID_ATTACK) {
		p_state = FADEOUT;
	}
	else if (timer > 255 && p_state == HIDING && mode == MERMAID_ATTACK) {
		mode = END;
		p_state = FADEIN;

		particles.clear();
		paragraph.clear();

		paragraph.push_back("Ithaka gave you the marvelous journey.");
		paragraph.push_back("Without her you would not have set out.");
		paragraph.push_back("She has nothing left to give you now.");

	}


	printf("%i - %f\n", p_state, alpha);
	switch (p_state) {
	case FADEIN:
		if (alpha >= 255) {
			p_state = SHOWING;
		}
		else {
			alpha += transparency_rate;
			ofSetColor(ofColor(255, 255, 255, alpha));
		}
		break;
	case FADEOUT:
		if (alpha <= 0) {
			p_state = HIDING;

		}
		else {
			alpha -= transparency_rate;
			ofSetColor(ofColor(255, 255, 255, alpha));
		}

		break;
	}

	int w = 640;
	int h = 480;
	ofMesh mesh;
	vector<float> distances;

	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 20;
	for (int y = 0; y < h; y += step) {
		for (int x = 0; x < w; x += step) {
			if (kinect.getDistanceAt(x, y) > 500 && kinect.getDistanceAt(x, y) < 1500) {
				//mesh.addColor(kinect.getColorAt(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
				distances.push_back(kinect.getDistanceAt(x, y));
			}
		}
	}



	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	ofVec2f nearestVertex;
	int nearestIndex = 0;

	ofVec3f centroid = mesh.getCentroid();

	for (int i = 0; i < n; i++) {
		ofVec3f cur = easyCam.worldToScreen(mesh.getVertex(i));
		float distance = cur.distance(ofPoint(centroid.x, -centroid.y));
		if (i == 0 || distance < nearestDistance) {
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
		}
	}
	
	ofPoint mass_center;

	if (distances.size() > 0 && mesh.getNumVertices() > 0) {
		//mass_center = ofPoint(ofMap(centroid.x, -ofGetWindowWidth() / 2, ofGetWindowWidth() / 2, 0, ofGetWindowWidth()), ofMap(distances.at(nearestIndex), 500, 1000, 0, ofGetWindowHeight()));
		mass_center = ofPoint(ofMap(centroid.x, -ofGetWindowWidth() / 2, ofGetWindowWidth() / 2, 0, ofGetWindowWidth()), ofMap(centroid.y, -ofGetWindowWidth() / 2, ofGetWindowWidth() / 2, 0, ofGetWindowHeight()));
	}
	else {
		//mass_center = ofPoint(ofGetWindowWidth()/2, ofGetWindowHeight() / 2);
	}
	mass = mass_center;
	
	
	for (int i = 0; i < particles.size(); ++i) {
		
		ofVec2f v = particles.at(i).position - mass_center;
		float dist = particles.at(i).position.distance(mass_center);




		switch (mode) {
		case SEA_CALM:
			if (dist < usr_radius) {
				particles.at(i).repultion(dist, usr_radius, v);
			}
			else {
				particles.at(i).moveToInit();
			}
			particles.at(i).moveLikeWave();
			break;
		case SEA_STORM:
			if (dist < usr_radius) {
				particles.at(i).repultion(dist, usr_radius, v);
			}
			else {
				particles.at(i).moveToInit();
			}
			particles.at(i).moveLikeWaveStorm();
			break;
		case CYCLOP:
			if (ofInRange(particles.at(i).init_position.distance(ofPoint(img_size / 2, img_size / 2)), dist - 0.05, dist + 0.05)) {
				particles.at(i).explode();
			}
			else {
				if (particles.at(i).force > 0) {
					particles.at(i).moveToDir();
					particles.at(i).force -= 0.001;
				}
				else {
					particles.at(i).traslateParticlesGroup(mass_center, img_size / 2);
				}
			}
			break;
		case MERMAID_CALM:
			if (dist < usr_radius) {
				particles.at(i).repel(usr_radius, mass_center);
			}
			else {
				particles.at(i).moveToDir();
			}
			break;
		case MERMAID_ATTACK:
			if (dist < usr_radius) {
				particles.at(i).repel(usr_radius, mass_center);
			}
			else {
				if (particles.at(i).force > 0) {
					particles.at(i).moveToDir();
					particles.at(i).force -= 0.001;
				}
				else {
					particles.at(i).attackTo(mass_center);
				}
			}
			break;
		case START:
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	if (mode == START) {
		for (size_t i = 0; i < paragraph.size(); i++) {
			font.drawString(paragraph.at(i), ofGetWindowWidth() / 2 - font.stringWidth(paragraph.at(i)) / 2, (ofGetWindowHeight() / paragraph.size()) * (i + 1) - font.stringHeight(paragraph.at(i)) / 2);
		}
	}
	else if (mode == END) {
		for (size_t i = 0; i < paragraph.size(); i++) {
			font.drawString(paragraph.at(i), ofGetWindowWidth() / 2 - font.stringWidth(paragraph.at(i)) / 2, ((ofGetWindowHeight() / 2) / paragraph.size()) * (i + 1) - font.stringHeight(paragraph.at(i)) / 2);
		}
	}
	else {
		for (int i = 0; i < particles.size(); ++i) {
			particles.at(i).draw();
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'x') {
		img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		img.save("screenshot.png");
	} if (key == 's') {
		ofToggleFullscreen();
	}
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
