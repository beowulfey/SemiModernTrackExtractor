/* 
 * File:   AutoThresholder.h
 * Author: Marc
 *
 * Created on November 11, 2009, 11:56 AM
 */

#ifndef _AUTOTHRESHOLDER_H
#define	_AUTOTHRESHOLDER_H

#include "cv.h"
#include "communicator.h"

class AutoThresholder {
public:
    static const bool messagesOn = true;
    AutoThresholder(IplImage *src, IplImage **dst, CvPoint refLocation, CvMemStorage *ms = NULL);
    AutoThresholder(IplImage *src, IplImage **dst, CvPoint2D32f refLocation, CvMemStorage *ms = NULL);
    
    virtual ~AutoThresholder();
    int toTargetArea(int targetArea,  int tstart = 128, int tlow = 1, int thigh = 255);
    int toMaxDLogArea();
    int toMaxDLogArea(int range_of_current);
    int toMaxDLogArea(int tlow, int thigh);

    double AutoThresholder::optimumArea(int minArea, int maxArea, int *threshold);
    double AutoThresholder::optimumArea(int minArea, int maxArea);

    inline int getThresh() {
        return thresh;
    }

    virtual void setMessageHandler (communicator *mh) {
        this->mh = mh;
    }
protected:
    communicator *mh;
    IplImage *src;
    IplImage *dst;
    CvPoint2D32f x0;
    int thresh;
    CvMemStorage *ms;
    bool freeMemStorageOnDestruction;
    void init(IplImage *src, IplImage **dst, CvPoint2D32f refLocation, CvMemStorage *ms = NULL);
    inline void message(const char *msg, const verbosityLevelT verb) {
        if (messagesOn && mh != NULL) {
            mh->message(name().c_str(), verb, msg);
        }
    }
    virtual inline string name() {
        return string("AutoThresholder");
    }
    vector<pair<int,double> > allAreas();
private:
    AutoThresholder(const AutoThresholder& orig);
    AutoThresholder();
    
};

#endif	/* _AUTOTHRESHOLDER_H */

