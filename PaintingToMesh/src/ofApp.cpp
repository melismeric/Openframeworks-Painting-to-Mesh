#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    btn1.addListener(this, &ofApp::btn1Pressed);
    btn2.addListener(this, &ofApp::btn2Pressed);
    btn3.addListener(this, &ofApp::btn3Pressed);
    btn4.addListener(this, &ofApp::btn4Pressed);
    btn5.addListener(this, &ofApp::btn5Pressed);
    
    gui.setup();
    gui.add(btn1.setup("A Wheatfield with Cypresses, 1889"));
    gui.add(btn2.setup("Self-Portrait, 1889"));
    gui.add(btn3.setup("Starry Night, 1889"));
    gui.add(btn4.setup("Sunflowers, 1888"));
    gui.add(btn5.setup("Almond Blossoms, 1890"));

    video.load("home.mp4");
}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    int numVerts = mesh.getNumVertices();
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i);

        float time = ofGetElapsedTimef();
        float timeScale = 5.0;
        float displacementScale = 0.75;
        ofVec3f timeOffsets = offsets[i];

        vert.x += (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
        vert.y += (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
        vert.z += (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
        mesh.setVertex(i, vert);
    }
    
    if (mouseDisplacement) {
        ofVec3f mouse(mouseX, ofGetHeight()-mouseY, 0);

        for (int i=0; i<mesh.getNumVertices(); ++i) {
            ofVec3f vertex = meshCopy.getVertex(i);
            float distanceToMouse = mouse.distance(vertex);
            float displacement = ofMap(distanceToMouse, 0, 400, 300.0, 0, true);
            ofVec3f direction = vertex - mouse;
            direction.normalize();
            ofVec3f displacedVertex = vertex + displacement*direction;
            mesh.setVertex(i, displacedVertex);
        }
    }
    
    if (orbiting) {
        int numVerts = mesh.getNumVertices();
        for (int i=0; i<numVerts; ++i) {
            ofVec3f vert = mesh.getVertex(i);
            float distance = distances[i];
            float angle = angles[i];
            float elapsedTime = ofGetElapsedTimef() - startOrbitTime;
            float speed = ofMap(distance, 0, 200, 1, 0.25, true);
            float rotatedAngle = elapsedTime * speed + angle;
            vert.x = distance * cos(rotatedAngle) + meshCentroid.x;
            vert.y = distance * sin(rotatedAngle) + meshCentroid.y;

            mesh.setVertex(i, vert);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofColor centerColor = ofColor(85, 78, 68);
    ofColor edgeColor(0, 0, 0);
    ofBackgroundGradient(0, edgeColor, OF_GRADIENT_CIRCULAR);
    video.draw(ofGetWidth()/2 - video.getWidth()/2, ofGetHeight()/2 - video.getHeight()/2);
    video.play();
    
    easyCam.begin();
        ofPushMatrix();
            ofScale(1, -1, 1);
            ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
            mesh.draw();
        ofPopMatrix();
    easyCam.end();
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'o') {
        orbiting = !orbiting; // This inverts the boolean
        startOrbitTime = ofGetElapsedTimef();
        mesh = meshCopy; // This restores the mesh to its original values
    }
    if (key == 'm') {
        mouseDisplacement = !mouseDisplacement; // Inverts the boolean
        mesh = meshCopy; // Restore the original mesh
    }
}
//--------------------------------------------------------------
void ofApp::imagetoMesh(string imageStr){
    video.close();
    mesh.clear();
    
    image.load(imageStr);
    image.resize(200, 200);
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
    mesh.enableColors();

    float intensityThreshold = 90.0;
    int w = image.getWidth();
    int h = image.getHeight();
    for (int x=0; x<w; ++x) {
        for (int y=0; y<h; ++y) {
            ofColor c = image.getColor(x, y);
            float intensity = c.getLightness();
            if (intensity >= intensityThreshold) {
                float saturation = c.getSaturation();
                float z = ofMap(saturation, 0, 255, -100, 100);
                ofVec3f pos(x*4, y*4, z);
                mesh.addVertex(pos);
                mesh.addColor(c);
                offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
            }
        }
    }
    
    mesh.setMode(OF_PRIMITIVE_LINES);

    mesh.enableIndices();
    float connectionDistance = 30;
    int numVerts = mesh.getNumVertices();
    for (int a=0; a<numVerts; ++a) {
        ofVec3f verta = mesh.getVertex(a);
        for (int b=a+1; b<numVerts; ++b) {
            ofVec3f vertb = mesh.getVertex(b);
            float distance = verta.distance(vertb);
            if (distance <= connectionDistance) {
                mesh.addIndex(a);
                mesh.addIndex(b);
            }
        }
    }

    meshCentroid = mesh.getCentroid();

    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i);
        float distance = vert.distance(meshCentroid);
        float angle = atan2(vert.y-meshCentroid.y, vert.x-meshCentroid.x);
        distances.push_back(distance);
        angles.push_back(angle);
    }

    orbiting = false;
    startOrbitTime = 0.0;
    meshCopy = mesh;
    
    mouseDisplacement = false;

}
//--------------------------------------------------------------
void ofApp::btn1Pressed(){
   imagetoMesh("seasons.jpg");
}
//--------------------------------------------------------------
void ofApp::btn2Pressed(){
    imagetoMesh("vangogh.jpg");
}
//--------------------------------------------------------------
void ofApp::btn3Pressed(){
    imagetoMesh("starrynight.jpg");
}
//--------------------------------------------------------------
void ofApp::btn4Pressed(){
    imagetoMesh("sunflowers.jpg");
}
//--------------------------------------------------------------
void ofApp::btn5Pressed(){
    imagetoMesh("almond.jpg");
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
