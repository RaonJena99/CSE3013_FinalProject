//
//  Slope.cpp
//  Tmp
//
//  Created by 이예준 on 6/25/24.
//

#include "Slope.h"

// 주어진 x 좌표에 대응하는 y 좌표를 계산하는 메서드
float Slope::getY(float x) const {
    if (x2 == x1) { // 경사면이 수직일 때는 y1을 반환
        return y1;
    } else {
        float slope = (y2 - y1) / (x2 - x1); // 경사 계산
        return y1 + slope * (x - x1); // 주어진 x에 대응하는 y 값을 반환
    }
}
