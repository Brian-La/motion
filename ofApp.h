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

//inherit Image from Shape
class Image : public Shape {
public:
    //constructors----------------------------------------------
    Image() {}      //empty
    Image(string filename) {
        load(filename);     //load filename;
    }
    
    //functions-------------------------------------------------
    //check if image loads
    void load(string filename) {
        
        //if image loads, assign values
        if(image.load(filename)) {
            width = image.getWidth();
            height = image.getHeight();
            bLoaded = true;
        }
        else        //else display error msg
            cout << "ERROR: Can't load image file: " << filename << endl;
    }
    
    bool inside(glm::vec3 p) {      //check if mouse inside image
        return (p.x > 0.0 && p.x < width && p.y > 0.0 && p.y < height);
    }
    
    void draw();        //override
    
    
    //data------------------------------------------------------
    ofImage image;          //image ofImage type
    bool bLoaded = false;       //bool check if image loaded
    float width, height;        //width and height of image
    
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
        glm::vec3 normPos;      //normalization of difference vectors
        glm::vec3 estPos;       //estimated position of triangle shift

        float curvePos = 0;     //position in sin wave
        
        
        //UI control------------------------------------------------
        bool bFullScreen = false;
        bool bCtrlKeyDown = false;
        bool bDrag = false;
        bool startAnim = false;
        
    
        //GUI-------------------------------------------------------
        ofxPanel gui;
    
        //float sliders: triangle speed, wave scale & cycles
        ofxFloatSlider animSpeed, animScale, animCycles;
    
        //imageToggle: if true ? image : triangle;
        //pathToggle:   if true ? path display : no path display
        //rainbowToggle
        ofxToggle imageToggle, pathToggle, rainbowToggle;
    
        Triangle tri;       //triangle obj
        Image img;          //image obj

    
};
