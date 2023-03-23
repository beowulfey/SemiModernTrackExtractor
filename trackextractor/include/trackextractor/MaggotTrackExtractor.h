/* 
 * File:   MaggotTrackExtractor.h
 * Author: Marc
 *
 * Created on November 6, 2009, 3:15 PM
 */

#ifndef _MAGGOTTRACKEXTRACTOR_H
#define	_MAGGOTTRACKEXTRACTOR_H

#include "TrackExtractor.h"

class MaggotTrackExtractor : public TrackExtractor{
public:
    MaggotTrackExtractor();
    virtual int go();

    inline void setMaxAngle (double ma) {
        maxAngle = 3.14159253 / 2;
    }
    inline double getMaxAngle () {
        return maxAngle;
    }
    void fromYAML(const YAML::Node &node);
protected:
    virtual void createTrackBuilder();
    virtual void createPointExtractor();
    double maxAngle;
    virtual YAML::Emitter& yamlBody(YAML::Emitter& out) const;

};
YAML::Emitter& operator << (YAML::Emitter& out, const MaggotTrackExtractor *te);
YAML::Emitter& operator << (YAML::Emitter& out, const MaggotTrackExtractor &te);
void operator >> (const YAML::Node &node, MaggotTrackExtractor * &te);
void operator >> (const YAML::Node &node, MaggotTrackExtractor (&te));
#endif	/* _MAGGOTTRACKEXTRACTOR_H */

