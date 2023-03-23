/* 
 * File:   ContourPoint.cpp
 * Author: Marc
 * 
 * Created on November 3, 2009, 9:40 AM
 *
 * like a track point but also stores an angle
 * for the far far future, maybe we should make a new kind of point that's
 * like a super CV point, but less than a track point.  then trackpoint could
 * subclass this one
 *
 * but CV 2.0 probably has this anyway
 */

#include "ContourPoint.h"

ContourPoint::ContourPoint(CvPoint pt, int ID) : TrackPoint(pt.x, pt.y, -1, NULL, -1, ID) {
    theta = NAN;
}
void ContourPoint::setAngle (const CvPoint prev, const CvPoint next) {
    theta = vertexAngle(ContourPoint(prev,0), ContourPoint(next,0));
}

void ContourPoint::setAngle (const ContourPoint prev, const ContourPoint next) {
     theta = vertexAngle(prev, next);
}

