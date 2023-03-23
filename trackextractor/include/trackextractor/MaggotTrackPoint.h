/* 
 * File:   MaggotTrackPoint.h
 * Author: Marc
 *
 * Created on October 30, 2009, 2:25 PM
 *
 * extends ImTrackPoint to also store a contour and information
 * about the head and tail location of the maggot
 * MaggotTrackPoints also contain pointers to the point ahead and behind them
 * useful for matching head/tail from frame to frame
 *
 * this class includes functions for extracting the contour, finding the head
 * and tail, and aligning the head/tail from frame to frame
 */


#ifndef _MAGGOTTRACKPOINT_H
#define	_MAGGOTTRACKPOINT_H
#include <string>
#include <vector>
#include "ImTrackPoint.h"
#include "cv.h"
//using namespace std;
using std::vector;
using std::string;

class MaggotTrackPoint : public ImTrackPoint {
public:
    /* MaggotTrackPoint(double x, double y, double area, double cov[],  int t, CvPoint offset, IplImage *src, int winSize);
     * MaggotTrackPoint(double x, double y, double area, double cov[],  int t, int ID, CvPoint offset, IplImage *src, int winSize);
     * MaggotTrackPoint(ImTrackPoint *pt) ;
     *
     * same basic constructors as an ImTrackPoint; if called on ImTrackPoint, we allocate new memory and copy the image over
     *
     * NB after construction, MTP does not have any additional information about the maggot (no contour info, etc.)
     * analyze() must be called to generate this information
     * (be sure to setAnalysisParams first)
     */
    MaggotTrackPoint(double x, double y, double area, double cov[],  int t, CvPoint offset, IplImage *src, int winSize);
    MaggotTrackPoint(double x, double y, double area, double cov[],  int t, int ID, CvPoint offset, IplImage *src, int winSize);
    MaggotTrackPoint(ImTrackPoint *pt) ;

    /* inline void setAnalysisParams (double targetArea, double maxAngle, double threshold)
     * 
     * targetArea -- if targetArea > 0, we attempt to set the threshold so that the contour has
     *      exactly targetArea area
     * maxAngle -- this is the maximum contour angle at the head/tail.  pi/2 seems to be a good value
     * threshold -- if targetArea < 0, we threshold to this value;  if targetArea > 0, we use this a starting point to
     *      when thresholding to targetArea, but it doesn't really matter much
     */
    inline void setAnalysisParams (double targetArea, double maxAngle, double threshold) {
        this->targetArea = targetArea;
        this->maxAngle = maxAngle;
        this->threshold = threshold;
    }
    /* inline void linkBehind(MaggotTrackPoint *prev)
     * inline void linkAhead(MaggotTrackPoint *next)
     * 
     * sets the previous and forward pointers for this MTP
     * linking behind causes the previous point to link ahead, but linking
     * ahead does not cause the next point to link behind
     */
    inline void linkBehind(MaggotTrackPoint *prev) {
        this->prev = prev;
        if (prev != NULL) {
            prev->linkAhead(this);
        }
    }
    inline void linkAhead(MaggotTrackPoint *next) {
        this->next = next;
    }
    /* void analyze()
     * re-thresholds the image, finds contour, head, tail, midpoint
     * aligns h/t with previous point (if prev != NULL) then attempts
     * to align all h/t of this and previous points with the direction of motion
     * 
     */
     void analyze();

     /*static bool convertITPToMTP (vector<TrackPoint *> *v)
      *
      * converts a vector of image track points to a vector of maggot track points (no analysis is done)
      * the previous ITPs are destroyed and new memory is allocated for the MTPs
      * if the points were already MaggotTrackPoints, they are unchanged except for being linked as below
      * the created MTPs are linked so that v[j+1]->prev = v[j]; v[j]->next = v[j+1]
      */
    static bool convertITPToMTP (vector<TrackPoint *> *v);

    /* static bool analyzeTrack (vector <TrackPoint *> *v, double minArea, double maxArea, double maxAngle)
     *
     * converts a vector of ImTrackPoints to MTPs then analyzes them;
     * first finds the optimum area (between minArea and maxArea) by calling optimumArea on each mtp, then taking
     * the median of this set of optimum areas
     *
     * maxAngle is the maximum included contour angle at which a HT location can be valid
     *
     * */
    static bool analyzeTrack (vector <TrackPoint *> *v, double minArea, double maxArea, double maxAngle);


    /* inline void linkAnalysisParams ()
     * sets the analysis params of this MTP to be the same as the previous
     * if prev == NULL, no effect
     */
    inline void linkAnalysisParams () {
        if (prev != NULL) setAnalysisParams(prev->targetArea, prev->maxAngle, prev->threshold);
    }
    

    inline void setMaxAngle (double ma) {
        maxAngle = ma;
    }
    inline double getMaxAngle () {
        return maxAngle;
    }

    
    virtual inline string name() {
        return string("MaggotTrackPoint");
    }
    /* virtual string saveDescription();
     * virtual int toDisk(FILE *f);
     * static TrackPoint *fromDisk(FILE *f);
     * virtual int sizeOnDisk();
     * virtual int makeMovieFrame (IplImage **dst);
     *
     * these functions override their counterparts in ImTrackPoint --
     * see description in ImTrackPoint.h
     */
     
    virtual string saveDescription();
    virtual int toDisk(FILE *f);
    static TrackPoint *fromDisk(FILE *f);
    virtual int sizeOnDisk();
    virtual int makeMovieFrame (IplImage **dst);
  
    

   

    virtual ~MaggotTrackPoint();
    virtual inline uchar getTypeCode() {
        return _id_code;
    }
    double optimumArea(double minArea, double maxArea, int *thr = NULL);
protected:
    void thresholdAndContour();
  //  void analyzeContour();
    void findHT();
    void matchHTWithPrev();
    void flipHTSegment();
    double htDPSegment(double prevSum); //compute the sum of the normalized dot products of tail-head vector and velocity vector for a segment
    MaggotTrackPoint *nextSegment();
    MaggotTrackPoint *segmentBeginning();
    void init();
    bool endOfSegment();
    

    double targetArea;
    double maxAngle;
    CvPoint head;
    CvPoint tail;
    CvPoint mid;
    double threshold;
    bool htValid;
    vector <CvPoint> contour;
    MaggotTrackPoint *prev;
    MaggotTrackPoint *next;

private:
    MaggotTrackPoint();
    MaggotTrackPoint(const MaggotTrackPoint& orig);
    static CvMemStorage *ms; //used when we find maggot contour, etc.
    static const uchar _id_code = 0x03;
    static bool findAndSetTargetArea(vector<TrackPoint*>* v, double minArea, double maxArea, double maxAngle);
};

#endif	/* _MAGGOTTRACKPOINT_H */

