/* 
 * File:   WormPointExtractor.h
 * Author: Marc
 *
 * Created on November 16, 2009, 4:18 PM
 */

#ifndef _WORMPOINTEXTRACTOR_H
#define	_WORMPOINTEXTRACTOR_H

#include <vector>
#include "PointExtractor.h"
#include "cv.h"

class WormPointExtractor : public PointExtractor {
public:
    WormPointExtractor();
    WormPointExtractor(const WormPointExtractor& orig);
    virtual ~WormPointExtractor();
    virtual inline void setStackLength(int stacklength) {
        this->stacklength = stacklength;
    }
    virtual inline int getStackLength () {
        return stacklength;
    }
    virtual inline void setDilationDist(int dist) {
        dilationDist = dist;
    }
    virtual inline int getDilationDist () {
        return dilationDist;
    }
    /* because we generate track points by looking at a motion blur
     * it doesn't make sense to split colliding points
     */
    virtual bool supportsCollisionSplits() {
        return false;
    }
protected:
    int stacklength;
    double hysteresisFactor;
    int dilationDist;
    std::vector<IplImage *> imstack;
    std::vector<TrackPoint *> lastPtsOut;
    IplImage *temporalThresh;
    IplImage *temporalSum;
    IplImage *temp32; //32 bit temporary image
    IplImage *baksub32;
    IplImage *curframe32;
    IplImage *bak32;
    IplImage *threshIm32;

    
    virtual void addFrameToStack();
    virtual void threshToZero();
    virtual inline const char *name() {
        return "Worm Point Extractor";
    }
   // virtual void averageOfImageStack(IplImage **dst);
    virtual void addNewImageToStack(IplImage *newImage);
private:
    void init();
};

#endif	/* _WORMPOINTEXTRACTOR_H */

