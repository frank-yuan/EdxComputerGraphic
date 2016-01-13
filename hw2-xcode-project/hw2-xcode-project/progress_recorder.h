//
//  progress_recorder.hpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/11/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef progress_recorder_h
#define progress_recorder_h

#include <stdio.h>


typedef void (*OnProgressChanged)(int progress);

class progress_recorder
{
public:
    progress_recorder(int steps):mTotalSteps(steps){};
    void UpdateProgress(int currentStep);
    inline int GetCurrentPercentage() const {return mCurrentSteps * 100 / mTotalSteps;}
    inline void RegisterChangedEvent(OnProgressChanged event){mChangedEvent = event;}
    
private:
    int mTotalSteps;
    int mCurrentSteps;
    OnProgressChanged mChangedEvent;
};
#endif /* progress_recorder_hpp */
