#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);


    m_kanna.load("kanna.png");
    m_kanna.setAnchorPoint(m_kanna.getWidth() / 2, m_kanna.getHeight() / 2);
    
    //object detected & lost variables
    m_detected_1 = false;
    m_detected_2 = false;
    m_loss_1 = false;
    m_loss_2 = false;

    m_tolerance_1 = 0;
    m_minRange_1 = 10;
    m_maxRange_1 = 150;

    m_tolerance_2 = 0;
    m_minRange_2 = 10;
    m_maxRange_2 = 150;

    //Intializing colour trackers color
    m_colorTracker_1 = ofColor::white;
    m_colorTracker_2 = ofColor::white;

    m_GUI.setup();

    if (USE_WEB_CAM) {
        m_webcam.setup(640, 480);
    }
    else {
        m_video.load("color_small.mov");            
        m_video.setLoopState(OF_LOOP_PALINDROME);   
        m_video.play();                             
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    //Update the contour tracker for colour 1
    m_contourTracker_1.setThreshold(m_tolerance_1);
    m_contourTracker_1.setMinAreaRadius(m_minRange_1);
    m_contourTracker_1.setMaxAreaRadius(m_maxRange_1);
    m_contourTracker_1.setTargetColor(m_colorTracker_1);

    //Update the contour tracker for colour 2
    m_contourTracker_2.setThreshold(m_tolerance_2);
    m_contourTracker_2.setMinAreaRadius(m_minRange_2);
    m_contourTracker_2.setMaxAreaRadius(m_maxRange_2);
    m_contourTracker_2.setTargetColor(m_colorTracker_2);

    if (USE_WEB_CAM) {
        m_webcam.update();

        if (m_webcam.isFrameNew()) {
            m_contourTracker_1.findContours(m_webcam);
            m_contourTracker_2.findContours(m_webcam);
        }
    }
    else {
        m_video.update();

        if (m_video.isFrameNew()) {
            m_contourTracker_1.findContours(m_video);
            m_contourTracker_2.findContours(m_video);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(ofColor::black);
    ofSetColor(ofColor::white);

    int numContours_1 = m_contourTracker_1.getContours().size();
    int numContours_2 = m_contourTracker_2.getContours().size();

    ofPushMatrix();
        //rectangle fill colour
        ofFill();
        ofSetColor(ofColor::black);
        ofDrawRectRounded(0, 500, 640, 150, 10);

        //rectangle border
        ofSetColor(ofColor::white);
        ofNoFill();
        ofDrawRectRounded(0, 500, 640, 150, 10);
    ofPopMatrix();
    
    if (USE_WEB_CAM) {
        m_webcam.draw(0, 0);
    }
    else {
        m_video.draw(0, 0);
    }

    drawContour(numContours_1, m_contourTracker_1, ofVec4f(255, 0, 0, 255), ofVec3f(0, 255, 0), &m_center_x1, &m_center_y1);
    drawContour(numContours_2, m_contourTracker_2, ofVec4f(255, 0, 255, 255), ofVec3f(0, 255, 255), &m_center_x2, &m_center_y2);

    colourDetected(numContours_1, &m_detected_1, &m_loss_1, ofVec3f(90, 525, 10));
    colourDetected(numContours_2, &m_detected_2, &m_loss_2, ofVec3f(210, 525, 10));

    calculateD();

    if (m_detected_1 && m_loss_1) {
        isColourLost(1);
    }
    if (m_detected_2 && m_loss_2) {
        isColourLost(2);
    }

    if (numContours_1 > 0 && numContours_2 > 0) {
        if (m_d <= 150 && m_d > 0) {
            /*cout << "Hit me baby one more time!" << endl;*/
            collision();
        }
    }

    //Shows the first colour that is being tracked
    ofPushMatrix();
        //Circle outline
        ofNoFill();
        ofSetColor(ofColor::white);
        ofSetCircleResolution(100);
        ofDrawCircle(50, 560, 30);

        //Text
        ofDrawBitmapString("Colour 1", 20, 620);

        //Circle Fill
        ofPushMatrix();
            ofFill();
            ofSetColor(m_colorTracker_1);
            ofDrawCircle(50, 560, 30);
        ofPopMatrix();
    ofPopMatrix();

    //Shows the second color that is being tracked
    ofPushMatrix();
        //Circle outline
        ofNoFill();
        ofSetColor(ofColor::white);
        ofSetCircleResolution(100);
        ofDrawCircle(170, 560, 30);

        //Text
        ofDrawBitmapString("Colour 2", 140, 620);

        //Circle 
        ofPushMatrix();
            ofFill();
            ofSetColor(m_colorTracker_2);
            ofDrawCircle(170, 560, 30);
        ofPopMatrix();
    ofPopMatrix();

    //draws the GUI
    m_GUI.begin();
        ImGui::Text("Hello, please click to select color");
        ImGui::Text("Colour 1");
        createGUI("C1", &m_tolerance_1, &m_minRange_1, &m_maxRange_1);
        
        ImGui::Text("Colour 2");
        createGUI("C2", &m_tolerance_2, &m_minRange_2, &m_maxRange_2);
    m_GUI.end();
}
 
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        if (!USE_WEB_CAM) {
            m_video.setPaused(!m_video.isPaused());
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    //right mouse click changes the first colour tracker
    if (button == OF_MOUSE_BUTTON_RIGHT) {
        if (USE_WEB_CAM) {
            if (x <= m_webcam.getWidth() && y <= m_webcam.getHeight()) {
                int index = (x + y * m_webcam.getWidth()) * m_webcam.getPixels().getBytesPerPixel();
                m_colorTracker_1.r = m_webcam.getPixels()[index + 0];
                m_colorTracker_1.g = m_webcam.getPixels()[index + 1];
                m_colorTracker_1.b = m_webcam.getPixels()[index + 2];
            }
        }
        else {
            int index = (x + y * m_video.getWidth()) * m_video.getPixels().getBytesPerPixel();
            m_colorTracker_1.r = m_video.getPixels()[index + 0];
            m_colorTracker_1.g = m_video.getPixels()[index + 1];
            m_colorTracker_1.b = m_video.getPixels()[index + 2];
        }
    }

    //middle mouse click changes the second colour tracker
    else if (button == OF_MOUSE_BUTTON_MIDDLE) {
        if (USE_WEB_CAM) {
            if (x <= m_webcam.getWidth() && y <= m_webcam.getHeight()) {
                int index = (x + y * m_webcam.getWidth()) * m_webcam.getPixels().getBytesPerPixel();
                m_colorTracker_2.r = m_webcam.getPixels()[index + 0];
                m_colorTracker_2.g = m_webcam.getPixels()[index + 1];
                m_colorTracker_2.b = m_webcam.getPixels()[index + 2];
            }
        }
        else {
            int index = (x + y * m_video.getWidth()) * m_video.getPixels().getBytesPerPixel();
            m_colorTracker_2.r = m_video.getPixels()[index + 0];
            m_colorTracker_2.g = m_video.getPixels()[index + 1];
            m_colorTracker_2.b = m_video.getPixels()[index + 2];
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawContour(int contNum, ofxCv::ContourFinder contTrack, ofVec4f rectCol, ofVec3f contRGB, int *centerX, int *centerY){

    for (int i = 0; i < contNum; i++) {
        cv::Point2f center = contTrack.getCenter(i);
        cv::Rect rect = contTrack.getBoundingRect(i);
        const std::vector<cv::Point> contour = contTrack.getContour(i);

        ofNoFill();
        ofSetColor(rectCol.x, rectCol.y, rectCol.z, rectCol.w);
        ofDrawRectangle(rect.x, rect.y, rect.width, rect.height);

        ofSetColor(contRGB.x, contRGB.y, contRGB.z);
        ofBeginShape();
            for (int j = 0; j < contour.size(); j++) {
                ofVertex(contour[j].x, contour[j].y);
            }
        ofEndShape(true);

        *centerX = center.x;    
        *centerY = center.y;    
    }
}

//--------------------------------------------------------------
void ofApp::colourDetected(int object, bool *hasDetect, bool *hasLost, ofVec3f geo) {
    if (object > 0) {
        ofPushMatrix();
            ofFill();
            ofSetCircleResolution(100);
            ofSetColor(ofColor::darkRed);
            ofDrawCircle(geo.x, geo.y, geo.z);
        ofPopMatrix();

        if (!*hasDetect) {
            *hasDetect = true;
        }
        else {
            *hasLost = false;
        }
    }
    if (*hasDetect && object == 0) {
        *hasLost = true;
    }
}

//--------------------------------------------------------------
void ofApp::isColourLost(int whichCol) {
    ofPushMatrix();
    ofSetColor(ofColor::darkRed);
    if (whichCol == 1) {
        ofDrawBitmapString("Colour 1 has been lost", 300, 550);
        m_center_x1 = NULL;
        m_center_y1 = NULL;
    }
    else if (whichCol == 2) {
        ofDrawBitmapString("Colour 2 has been lost", 300, 590);
        m_center_x2 = NULL;
        m_center_y2 = NULL;
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::calculateD() {
    double x = m_center_x2 - m_center_x1;
    double y = m_center_y2 - m_center_y1;
    m_d = sqrt(pow(x,2) + pow(y,2));
}

void ofApp::collision(){
    ofPushMatrix();
        ofSetBackgroundColor(ofColor::darkBlue);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::createGUI(string colNum, int *tolerance, float *min, float *max) {

    //concatenating the colour number to the base label
    string TOL = colNum + ": Tolerance";
    string MIN = colNum + ": Min Radius";
    string MAX = colNum + ": Max Radius";

    ImGui::SliderInt(TOL.c_str(), tolerance, 0, 255);
    ImGui::SliderFloat(MIN.c_str(), min, 0, 500);
    ImGui::SliderFloat(MAX.c_str(), max, 0, 500);
}


