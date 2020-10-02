#include "ofApp.h"

ofVec2f tVec, iVec, cVec;   //angle vectors: triangle (t), image (i), curve point (c)

//override Triangle draw method: use mult matrix
void Triangle::draw() {
    ofPushMatrix();
    ofMultMatrix(getMatrix());
    ofDrawTriangle(verts[0], verts[1], verts[2]);
    ofPopMatrix();
}

//check to see if mouse position inside triangle
bool Triangle::inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    //normalize vectors
    glm::vec3 v1 = glm::normalize(p1 - p);
    glm::vec3 v2 = glm::normalize(p2 - p);
    glm::vec3 v3 = glm::normalize(p3 - p);
    
    //assign float values
    float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
    float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
    float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
    
    //works clock-wise or counter clock-wise orientation
    //if "a" values are negative, return true
    if(signbit(a1) == signbit(a2) && signbit(a2) == signbit(a3))
        return true;
    else
        return false;
}

//override Image draw method:: use mult matrix
void Image::draw(){
    ofPushMatrix();
    ofMultMatrix(getMatrix());
    ofScale(0.05, 0.05, 0);     //scale image down first
    image.draw(-width/2.0, -height/2.0, 0);     //center image post TLM
    ofPopMatrix();
}


//--------------------------------------------------------------
void ofApp::setup(){

    //assign default tri position = middle of window
    tri.pos = glm::vec3(ofGetWidth()/2.0, ofGetHeight()/2.0, 0);
    
    //push vertices based on post-matrix calculations
    tri.verts.push_back(glm::vec3(-20, -20, 0));    //left
    tri.verts.push_back(glm::vec3(0, 40, 0));      //middle
    tri.verts.push_back(glm::vec3(20, -20, 0));     //right
    
    //setup GUI sliders and buttons
    gui.setup();
    gui.add(animScale.setup("Scale", 200, 0, 400));     //speed default: 200
    gui.add(animCycles.setup("Cycles", 4, 0, 10));      //... default: 4
    gui.add(animSpeed.setup("Speed", 6, 1, 20));      //... default: 4
    gui.add(imageToggle.setup("Use image", false));     //...default false
    gui.add(pathToggle.setup("Draw path", true));       //...default true
    gui.add(rainbowToggle.setup("Color Frenzy", false));     //false
    
    
    //load image and center position
    img.load("images/pacman.png");
    img.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
    
    
    ofSetBackgroundColor(ofColor::black);
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    //start animation
    if(startAnim) {
        //set triangle color orange
        tri.color = ofColor::orange;
        
        //movement (constant speed)-----------
        //reset curvePos when img or tri reach end of window
        if(tri.pos.x >= ofGetWidth() || img.pos.x >= ofGetWidth()) curvePos = 0;
        
        //calc. normal vector for mag./dir. of next point on curve
        normPos = glm::normalize(curveEval(curvePos + 1, animScale, animCycles) - tri.pos);
        
        //multiply normal vector by dist desired (i.e. speed)
        estPos = normPos * float(animSpeed);
        
        //increment next tri pos to est. pos based on multiplied normal vector x value
        curvePos += estPos.x;

        //set tri pos to new curve pos
        tri.pos = curveEval(curvePos, animScale, animCycles);
        img.pos = curveEval(curvePos, animScale, animCycles);
        
        
        //rotation----------------------------
        tVec = glm::vec3(0, -1, 0);       //set an initial heading vector (h.v.) thru TLM for triangle
        iVec = glm::vec3(-1, 0, 0);        //initial h.v.
        cVec = glm::vec3(tri.pos - curveEval(curvePos + 1, animScale, animCycles)); //acquire next h.v. thru. diff. of pos between tri pos and curvePos
        
        
        tri.rotation = tVec.angle(cVec);    //set tri rotation to angle between h.v.'s
        img.rotation = iVec.angle(cVec);      //...img
        
    }

}


//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();     //draw gui
    
    //check toggle button
    if(!imageToggle) {
        ofSetColor(tri.color);     //set color of triangle
        tri.draw();     //draw triangle
    }
    else
        img.draw();
    
    
    //draw sin wave if path toggle true
    if(pathToggle) {
        ofSetColor(ofColor::white);
        
        for(int i = 0; i < ofGetWidth(); i+=5) {
            if(rainbowToggle)
                ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
            glm::vec3 p = curveEval(i, animScale, animCycles);
            ofDrawCircle(p.x, p.y, 2);      //circle each pixel
        }
    }
    
}


//draw sin wave across window width per pixel
//x = pixel, scale = amplitude/height, cycles = wavelength
glm::vec3 ofApp::curveEval(float x, float scale, float cycles) {
    float u = (cycles * x * PI) / ofGetWidth();
    return (glm::vec3(x, -scale * sin(u) + (ofGetHeight()/2), 0));
}


//rotate all vertices from vin and push results to vout
void ofApp::rotate(const vector<glm::vec3> &vin, vector<glm::vec3> &vout, float angle) {
    for(int i = 0; i < vin.size(); i++) {
        vout.push_back(glm::vec3(vin[i].x * cos(angle) - vin[i].y * sin(angle), vin[i].x * sin(angle) + vin[i].y * cos(angle), 0));
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    //option based on key pressed using switch statement
    switch(key) {
        case 'f':
            bFullScreen = !bFullScreen;       //toggle fullscreen
            ofSetFullscreen(bFullScreen);
            break;
        case OF_KEY_CONTROL:        //turn on key switch
            bCtrlKeyDown = true;
            break;
        case ' ':       //space
            startAnim = !startAnim;     //toggle animation
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch(key) {
        case OF_KEY_CONTROL:
            bCtrlKeyDown = false;   //reset ctrl switch
            break;
        default:
            break;
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    //exit conditional if bDrag isn't on
    if(!bDrag) return;
    
    //current mouse position drag
    glm::vec3 mousePoint(x, y, 0);
    
    if(bCtrlKeyDown) {
        tri.rotation += x - lastMouse.x;    //diff. of mouse position adjusted to rotation variable
        tri.color = ofColor::blue;
    }
    else {
        tri.pos += mousePoint - lastMouse;      //if not rotate, adjust tri position by diff of mouse positions
        tri.color = ofColor::green;
    }
    lastMouse = mousePoint;     //set previous to current

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    vector<glm::vec3> points;       //new vector: vout
    rotate(tri.verts, points, glm::radians(tri.rotation));      //rotate and set in vout
    
    //if mouse inside new rotated triangle, set drag true and last mouse position
    if(tri.inside(glm::vec3(x, y, 0), tri.pos + points[0], tri.pos + points[1], tri.pos + points[2])) {
        bDrag = true;
        lastMouse = glm::vec3(x, y, 0);
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bDrag = false;       //drag false
    tri.color = ofColor::red;
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
