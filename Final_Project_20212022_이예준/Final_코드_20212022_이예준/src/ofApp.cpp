//
//  ofApp.cpp
//  Tmp
//
//  Created by 이예준 on 6/25/24.
//
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // 배경색 지정
    ofBackground(255, 255, 255);
    
    // 각 Flag 초기화
    isMousePressed = false;
    load_flag = false;
    draw_flag = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    // 마우스가 눌렸을 때
    if (isMousePressed) {
        // 5개의 원을 생성
        for (int r = 0; r < 5; r++) {
            Trickle circle;
            // 마우스의 중심에서 일정 범위까지의 무작위 위치를 지정
            float xOffset = ofRandom(-20, 20);
            float yOffset = ofRandom(-20, 20);
            // 화면 위아래에 빨간 띠가 존재하기 때문에 위쪽 빨간 띠와 겹치는 경우 생성하지 않음
            if(mouseY + yOffset < 40) continue;
            // 마우스 중심에서 이동
            circle.x = mouseX + xOffset;
            circle.y = mouseY + yOffset;
            circle.onSlope = false;
            // 최종적으로 결정된 윈의 위치를 water vector에 저장
            water.push_back(circle);
        }
    }
    
    // vector 배열 water의 위치를 업데이트
    for (auto& circle : water) {
        // 기본 셋
        circle.onSlope = false;
        
        // 만약 파일의 경사면을 화면에 그린 경우
        if(draw_flag){
            for (const auto& slope : slopes) {
                // x축 기준으로 원이 경사면 사이에 존재하는지 체크
                if(circle.x <= slope.x2 && circle.x >= slope.x1){
                    // y축 기준으로 해당 경사면과 상호작용이 일어났는지 체크
                    if(circle.y >= slope.getY(circle.x)-3 && circle.y < slope.getY(circle.x)-1){
                        circle.onSlope = true;
                        break;
                    }
                }
            }
        }
        //만약 화면에 경사면이 존재하지 않는 경우
        if (!circle.onSlope) {
            circle.y += 2;
        }
        //만약 화면에 경사면이 존재하는 경우
        else {
            for (const auto& slope : slopes) {
                // x축 기준으로 원이 경사면 사이에 존재하는지 체크
                if (circle.x <= slope.x2 && circle.x >= slope.x1) {
                    // y축 기준으로 해당 경사면과 상호작용이 일어났는지 체크
                    if(circle.y >= slope.getY(circle.x)-3 && circle.y < slope.getY(circle.x)-1){
                        // 경사면의 각도 계산
                        float slopeAngle = atan2(slope.y2 - slope.y1, slope.x2 - slope.x1);
                        // 만약 경사면의 오른쪽이 더 올라갔다면 물의 방향은 왼쪽
                        if(slope.y2 < slope.y1) circle.x -= (cos(slopeAngle) * 2);
                        // 만약 경사면의 왼쪽이 더 올라갔다면 물의 방향은 오른쪽
                        else circle.x += (cos(slopeAngle) * 2);
                        //경사면을 따라서 이동
                        circle.y += (sin(slopeAngle) * 2);
                        break;
                    }
                }
            }
        }
    }
    
    water.erase(
        remove_if(water.begin(), water.end(), [](const Trickle& circle) {
            //화면의 아래쪽에 빨간 띠가 존재하기 때문에 빨간띠와 겹치게 되면 제거
            return circle.y > ofGetHeight()-40;
        }),
        water.end()
    );
}

//--------------------------------------------------------------
void ofApp::draw(){
    //빨간 띠의 색 설정
    ofSetColor(127, 23, 31);
    //위 아래로 그림
    ofDrawRectangle(0, 0, 1024, 40);
    ofDrawRectangle(0, 728, 1024, 40);
    
    //물의 색 설정
    ofSetColor(ofColor::blue); // draw everything in red
    for (const auto& circle : water) {
        // 물방울 크기를 무작위로 설정 후 그리기
        int size = ofRandom(1, 3);
        ofDrawCircle(circle.x, circle.y, size);
    }

    //만약 파일을 받아왔다면
    if(draw_flag){
        //선의 굵기 및 색 설정
        ofSetLineWidth(3);
        ofSetColor(ofColor::black);
        //파일에서 받아온 모든 선들의 정보를 이용해서 그림
        for (const auto& slope : slopes) {
            ofDrawLine(slope.x1, slope.y1, slope.x2, slope.y2);
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // 'v'를 누르는 경우 스크린샷을 저장
    if (key == 'v') {
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
    }
    // 'q'를 누르는 경우 프로그램 종료
    if (key == 'q') {
        _Exit(0);
    }
    // 'd'를 누르는 경우 경사면 그리기 허용
    if (key == 'd') {
        if (!load_flag){
            cout<<"Please load the file first"<<endl;
            return;
        }
        draw_flag = true;
    }
    // 'e'를 누르는 경우 경사면 삭제
    if (key == 'e') {
        draw_flag = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // 'l'을 누르는 경우 파일 받아오기
    if (key == 'l') {
        // Open the Open File Dialog
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

        // Check whether the user opened a file
        if (openFileResult.bSuccess) {
            ofLogVerbose("User selected a file");

            processOpenFileSelection(openFileResult);
            load_flag = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    //마우스를 드래그하면 마우스의 좌표를 저장
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //마우스를 클릭하면 마우스의 좌표를 저장
    isMousePressed = true;
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //마우스를 클릭하지 않으면 플래그 해제
    isMousePressed = false;
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

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

//--------------------------------------------------------------
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    // 파일이 존재하지 않는 경우
    if (!file.exists()) {
        cout << "Target file does not exists." << endl;
    }
    // 파일이 존재하는 경우
    else {
        cout << "We found the target file." << endl;
    }

    // 파일을 버퍼로 읽어옴
    ofBuffer buffer(file);
    int idx = 0;

    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        // 한 줄씩 읽어옴
        string line = *it;
        // 공백으로 문자열을 분리
        vector<string> words = ofSplitString(line, " ");
        
        if (words.size() == 1) {
            // 경사면의 개수 저장
            int num_of_line = stoi(words[0]);
            // 경사면의 개수 출력
            cout << "The number of lines is: " << num_of_line << endl;
            slopes.resize(num_of_line);
        } else if (words.size() >= 2) {
            // 경사면의 양 끝 2개의 x, y좌표 저장
            slopes[idx].x1 = stoi(words[0]);
            slopes[idx].y1 = stoi(words[1]);
            slopes[idx].x2 = stoi(words[2]);
            slopes[idx].y2 = stoi(words[3]);
            // 좌표가 화면 범위를 벗어나는지 체크
            if (slopes[idx].x1 < 0 || slopes[idx].x1 > ofGetWidth() ||
                slopes[idx].y1 < 0 || slopes[idx].y1 > ofGetHeight() ||
                slopes[idx].x2 < 0 || slopes[idx].x2 > ofGetWidth() ||
                slopes[idx].y2 < 0 || slopes[idx].y2 > ofGetHeight()) {
                cout << "Out-of-range value detected." << endl;
                return;
            }
            idx++;
        }
    }
}
