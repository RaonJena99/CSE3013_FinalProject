//
//  ofApp.h
//  Tmp
//
//  Created by 이예준 on 6/25/24.
//
#pragma once

#include "ofMain.h"
#include "Slope.h"

// 물 방울을 나타내는 구조체
struct Trickle {
    float x;
    float y;
    bool onSlope; // 경사면 위에 있는지 여부
};

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
    
        // 파일 열기 다이얼로그 처리 메서드
        void processOpenFileSelection(ofFileDialogResult openFileResult);
    
        vector<Trickle> water;  // 물 방울들을 저장하는 벡터
        vector<Slope> slopes;   // 경사면들을 저장하는 벡터
        bool isMousePressed;    // 마우스 버튼 눌림 여부
        bool load_flag;         // 파일 로드 여부 플래그
        bool draw_flag;         // 경사면 그리기 여부 플래그
};
