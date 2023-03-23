/* 
 * File:   MaggotTrackBuilder.h
 * Author: Marc
 *
 * Created on November 6, 2009, 10:34 AM
 *
 * a simple subclass of TrackBuilder that converts tracks to maggot tracks
 * and analyzes them
 */

#include "TrackBuilder.h"
#include "MaggotTrackPoint.h"


#ifndef _MAGGOTTRACKBUILDER_H
#define	_MAGGOTTRACKBUILDER_H

class MaggotTrackBuilder : public TrackBuilder {
public:
    MaggotTrackBuilder();
    MaggotTrackBuilder(int startFrame);
    
    /* void analyzeMaggotTracks(double targetArea, double maxAngle, double threshold);
     *
     * converts all tracks (active and finished) to maggotTracks and analyzes them according to
     * params minArea, maxArea, maxAngle. (see MaggotTrackPoint.h for details)
     */
    void analyzeMaggotTracks(double minArea, double MaxArea, double maxAngle);

    static MaggotTrackBuilder *fromDisk (FILE *f, TrackPoint *(*ptLoader) (FILE *f) = NULL );
protected:
    inline virtual uchar my_type_code () {
        return _id_code;
    }
private:
     static const uchar _id_code = 0x02;

};

#endif	/* _MAGGOTTRACKBUILDER_H */

