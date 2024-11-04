//
//  Slope.h
//  Tmp
//
//  Created by 이예준 on 6/25/24.
//

#ifndef Slope_hpp
#define Slope_hpp

#include "ofMain.h"

// 경사면을 나타내는 구조체
class Slope {
public:
    float x1, y1, x2, y2;
    float getY(float x) const; // Declare the method as const
};

#endif /* Slope_hpp */
