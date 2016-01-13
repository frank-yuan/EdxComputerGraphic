//
//  defines.h
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/7/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef defines_h
#define defines_h
#define PI 3.1415926
#define FLOAT_PRECISION 0.00001f
#define IS_FLOAT_EQUALS(X, Y) ((X - Y >= 0 ? X - Y : Y - X) < FLOAT_PRECISION)
#define ABS(X) (X >= 0 ? X : -X)

#endif /* defines_h */
