// Amy Friedman copyright Feb 2014
//IACD Carnegie Mellon University
//combination of examples from Kyle McDonald and Golan Levin
#include "ofApp.h"

const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

ofVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
    return ofVec3f((x / Xres - .5f) * z * XtoZ,
                   (y / Yres - .5f) * z * YtoZ,
                   z);
}

void exportPlyCloud(string filename, ofMesh& cloud) {
    ofFile ply;
    if (ply.open(filename, ofFile::WriteOnly)) {
        // write the header
        ply << "ply" << endl;
        ply << "format binary_little_endian 1.0" << endl;
        ply << "element vertex " << cloud.getVertices().size() << endl;
        ply << "property float x" << endl;
        ply << "property float y" << endl;
        ply << "property float z" << endl;
        ply << "end_header" << endl;
        
        // write all the vertices
        vector<ofVec3f>& surface = cloud.getVertices();
        for(int i = 0; i < surface.size(); i++) {
            if (surface[i].z != 0) {
                // write the raw data as if it were a stream of bytes
                ply.write((char*) &surface[i], sizeof(ofVec3f));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::setup(){

    
    panel.setup(280, 280);
    panel.addPanel("Settings");
    panel.addSlider("nearClipping", 500, 500, 40000);
    panel.addSlider("farClipping", 2000, 500, 40000);
   // float nearClipping = panel.getValueF("nearClipping");
    //float farClipping = panel.getValueF("farClipping");
    
    ofSetVerticalSync(true);
    kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
    kinect.open();
   // kinect.setDepthClipping(nearClipping, farClipping);


}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    
    float nearClipping = panel.getValueF("nearClipping");
    float farClipping = panel.getValueF("farClipping");
    kinect.setDepthClipping(nearClipping, farClipping);
    
    //in ptcexample this is placed in draw section
    if(kinect.isFrameNew()){
        int width = kinect.getWidth();
        int height = kinect.getHeight();
        float* distancePixels = kinect.getDistancePixels();// distance in centimeters
        
        cloud.clear();
        cloud.setMode(OF_PRIMITIVE_POINTS);
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                int i = y * width + x;
                float z = distancePixels[i];
                if(z != 0) { // ignore empty depth pixels
                    cloud.addVertex(ConvertProjectiveToRealWorld(x, y, z));
                }
            }
        }
    }
    if(exportPly) {
        exportPlyCloud("cloud.ply", cloud);
        ofDrawBitmapString("Point cloud Exported", 10, 20);
        exportPly = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);

    ofSetColor(255);
    
    easyCam.begin();
    ofScale(1, -1, -1); // orient the point cloud properly
    ofPushMatrix();
    ofTranslate(500,500);
    ofTranslate(0, 0, -150); // rotate about z = 150 cm
   // cloud.drawVertices();
    ofPopMatrix();
    easyCam.end();
   // ofDrawBitmapString("Point cloud View", 500, 750);
    

    kinect.drawDepth(300, 100, 700, 600);
    ofDrawBitmapString("Clipped View", 10, 400);
    ofDrawBitmapString("Press any key to Export", 10, 430);


}

//--------------------------------------------------------------
void ofApp::exit(){
    kinect.close();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        exportPly = true;
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
