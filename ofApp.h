#pragma once

#include "ofMain.h"
#include "ofxGui.h"


//general class Shape
class Shape {
public:
    //functions-------------------------------------------------
    Shape() {}  //construct
    virtual void draw() {}      //override draw
    virtual bool inside (glm::vec3 p) { return false; }     //override inside
    
    //define getMatrix()
    glm::mat4 getMatrix() {
        glm::mat4 t = glm::translate(glm::mat4(1.0), glm::vec3(pos));    //translate IMT to pos
        glm::mat4 r = glm::rotate(t, glm::radians(rotation), glm::vec3(0, 0, 1));   //rotate Z axis
        
        return r;   //return last matrix for multiplication
    }
    
    //data------------------------------------------------------
    vector<glm::vec3> verts;        //store vertices
    glm::vec3 pos;      //position of shape
    float rotation = 0;     //degrees variable
    ofColor color = ofColor::red;          //color of shape
};

//inherit Triangle from Shape
class Triangle : public Shape {
public:
    bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);       //override
    void draw();        //override
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
        
        //return sin wave position at every pixel
        glm::vec3 curveEval(float x, float scale, float cycles);
    
        //rotate (angle) vertices (vin) by an angle and return (vout)
        void rotate(const vector<glm::vec3> &vin, vector<glm::vec3> &vout, float angle);
		
        //app data--------------------------------------------------
        glm::vec3 lastMouse;    //save previous location of mouse movement

        float curvePos = 0;     //position in sin wave
        
        
        //UI control------------------------------------------------
        bool bFullScreen = false;
        bool bCtrlKeyDown = false;
        bool bDrag = false;
        bool startAnim = false;
        
    
        //GUI-------------------------------------------------------
        ofxPanel gui;
    
        //speed of triangle
        ofxFloatSlider animSpeed, animScale, animCycles;
        ofxButton s;
    
        Triangle tri;       //triangle obj
        ofImage pac;        //image

    
};
