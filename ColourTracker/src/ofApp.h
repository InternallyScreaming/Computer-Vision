#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxImGui.h"
#include <math.h> 

#define USE_WEB_CAM true

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        ofImage                 m_kanna;

        bool                    m_detected_1;
        bool                    m_loss_1;
        bool                    m_detected_2;
        bool                    m_loss_2;

        double                  m_d;
        int                     m_center_x1;    
        int                     m_center_y1;    
        int                     m_center_x2;    
        int                     m_center_y2; 

        std::vector<cv::Point>  m_contour_1;
        std::vector<cv::Point>  m_contour_2;

        int                     m_tolerance_1;
        float                   m_minRange_1;
        float                   m_maxRange_1;
        ofColor                 m_colorTracker_1;
        ofxCv::ContourFinder    m_contourTracker_1;


        int                     m_tolerance_2;
        float                   m_minRange_2;
        float                   m_maxRange_2;
        ofColor                 m_colorTracker_2;
        ofxCv::ContourFinder    m_contourTracker_2;

        ofxImGui::Gui           m_GUI;
        ofVideoPlayer           m_video;
        ofVideoGrabber          m_webcam;

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
        void drawContour(int contNum, ofxCv::ContourFinder contTrack, ofVec4f rectCol, ofVec3f contRGB, int *centerX, int *centerY);
        void colourDetected(int object, bool *hasDetect, bool *hasLost, ofVec3f geo);
        void isColourLost(int whichCol);
        void calculateD();
        void collision();
        void createGUI(string colNum, int *tolerance, float *min, float *max);
};
