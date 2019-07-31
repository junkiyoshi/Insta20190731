#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(1);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	auto radius = 30;
	auto len = 5;
	auto span = 120;
	auto threshold = 120;

	vector<glm::vec2> locations;
	for (auto x = span * 0.5; x < ofGetWidth(); x += span) {

		for (auto y = span * 0.5; y < ofGetHeight(); y += span) {

			auto location = glm::vec2(
				x + ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -span * 0.5 + radius, span * 0.5 - radius),
				y + ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -span * 0.5 + radius, span * 0.5 - radius));
			locations.push_back(location);
		}
	}

	for (auto location : locations) {

		vector<glm::vec2> vertices_1, vertices_2;
		for (auto deg = 0; deg < 360; deg++) {

			vertices_1.push_back(glm::vec2(location.x + radius * cos(deg * DEG_TO_RAD), location.y + radius * sin(deg * DEG_TO_RAD)));
			vertices_2.push_back(glm::vec2(location.x + (radius - len) * cos(deg * DEG_TO_RAD), location.y + (radius - len) * sin(deg * DEG_TO_RAD)));
		}

		ofNoFill();

		ofBeginShape();
		ofVertices(vertices_1);
		ofEndShape(true);

		ofBeginShape();
		ofVertices(vertices_2);
		ofEndShape(true);

		for (auto other : locations) {

			if (location == other) { continue; }

			auto distance = glm::distance(location, other);
			if (distance < threshold) {

				auto direction_rad = std::atan2(other.y - location.y, other.x - location.x);
				auto direction = direction_rad * RAD_TO_DEG;
				auto width = ofMap(distance, 0, threshold, 360, 0);

				vector<glm::vec2> in, out;
				for (auto deg = direction - width * 0.5; deg <= direction + width * 0.5; deg++) {

					in.push_back(glm::vec2(location.x + (radius - len) *cos(deg * DEG_TO_RAD), location.y + (radius - len) * sin(deg * DEG_TO_RAD)));
					out.push_back(glm::vec2(location.x + radius * cos(deg * DEG_TO_RAD), location.y + radius * sin(deg * DEG_TO_RAD)));
				}

				reverse(out.begin(), out.end());

				ofFill();
				ofBeginShape();
				ofVertices(in);
				ofVertices(out);
				ofEndShape();
				
				ofDrawLine(location + glm::vec2(radius * cos(direction * DEG_TO_RAD), radius * sin(direction * DEG_TO_RAD)),
					other + glm::vec2(radius * cos((180 + direction) * DEG_TO_RAD), radius * sin((180 + direction) * DEG_TO_RAD)));
			}
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}