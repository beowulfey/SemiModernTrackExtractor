/* 
 * File:   MaggotTrackBuilder.cpp
 * Author: Marc
 * 
 * Created on November 6, 2009, 10:34 AM
 */
#include <vector>
#include "Track.h"
#include "TrackBuilder.h"
#include "MaggotTrackBuilder.h"
#include "MaggotTrackPoint.h"
#include "tictoc.h"

using namespace std;

MaggotTrackBuilder::MaggotTrackBuilder() {
}
MaggotTrackBuilder::MaggotTrackBuilder(int startFrame) : TrackBuilder(startFrame){
    
}
void MaggotTrackBuilder::analyzeMaggotTracks(double minArea, double maxArea, double maxAngle) {
    //vector<Track *>::iterator it;
    _TICTOC_TIC_FUNC;
    message("AMT called", verb_debug);
    int j = 0;
    for (vector<Track *>::iterator it = activetracks.begin(); it != activetracks.end(); ++it){
        stringstream s; s << "track # " << ++j << "is an active ";
        s << (*it)->description();
        message (s.str().c_str(), verb_message);
        cout << j <<"/" <<activetracks.size() + finishedtracks.size() << "\t";
        MaggotTrackPoint::analyzeTrack((*it)->getPointVector(), minArea, maxArea, maxAngle);
    }
    for (vector<Track *>::iterator it = finishedtracks.begin(); it != finishedtracks.end(); ++it) {
        
        stringstream s; s << "track # " << ++j << "is a finished ";
        s << (*it)->description();
        message (s.str().c_str(), verb_message);
        cout << j <<"/" <<activetracks.size() + finishedtracks.size() << "\t";
        MaggotTrackPoint::analyzeTrack((*it)->getPointVector(), minArea, maxArea, maxAngle);
    }
    _TICTOC_TOC_FUNC;
}

MaggotTrackBuilder *MaggotTrackBuilder::fromDisk (FILE *f, TrackPoint *(*ptLoader) (FILE *f) ) {
    MaggotTrackBuilder *tb = new MaggotTrackBuilder;
    if (tb->loadTracksFromDisk(f, ptLoader) != 0) {
        delete tb;
        return NULL;
    } else {
        return tb;
    }
}