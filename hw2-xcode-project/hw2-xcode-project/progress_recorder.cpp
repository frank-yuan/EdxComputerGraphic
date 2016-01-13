//
//  progress_recorder.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/11/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include "progress_recorder.h"

void progress_recorder::UpdateProgress(int currentStep)
{
    if (currentStep != mCurrentSteps)
    {
        int lastValue = mCurrentSteps * 100 / mTotalSteps;
        int currentValue = currentStep * 100 / mTotalSteps;
        if (lastValue != currentValue)
        {
            if (mChangedEvent != NULL)
            {
                mChangedEvent(currentValue);
            }
        }
        mCurrentSteps = currentStep;
    }
}