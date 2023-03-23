/* 
 * File:   mycvutilites.h
 * Author: Marc
 *
 * Created on December 22, 2008, 1:30 PM
 */

#ifndef _CV_UTILS_PLUS_PLUS_H
#define	_CV_UTILS_PLUS_PLUS_H


#include "cv.h"
#include "cxcore.h"
#include "cxtypes.h"
#include "highgui.h"
#include <assert.h>
#include <vector>

#define BAD_FRAME_VALUE -100
#define FRAME_LOAD_ERROR -101
    
typedef struct _MovieT {
    CvCapture *video;
    CvSize frame_size;
    long int num_frames;
} MovieT;

/*sizeEqual
 *
 * returns 1 iff sz1.width and sz1.height are the same as sz2*/
int sizeEqual (CvSize sz1, CvSize sz2);

/*allocateImage
 *
 *calls cvCreateImage to create an image and its data
 *if *img is already allocated, checks to see if the size, depth, and nchannels are the same
 *if they are, returns without making any changes
 *if they are not, calls cvReleaseImage to free *img's data and allocates new storage*/
void allocateImage (IplImage **img, CvSize size, int depth, int channels);

void allocateImage (IplImage **img, const IplImage *src);
/*void imCopy (const IplImage *src, IplImage **dst)
 *
 *(re)allocates memory for dst, then copies src into dst
 */

void imCopy (const IplImage *src, IplImage **dst);
/*getNumFrames
 *
 *returns the number of frames in a loaded video*/
long int getNumFrames (CvCapture *video);

/*loadFrame
 *
 * loads frame frame_num from movie mov as a single channel 8 bit image
 * if frame_num < 0, loads image from current location and increments current location
 * by 1
 * if *im is correctly allocated, copies frame into image; otherwise releases im's storage
 * and reallocates
 */
int loadFrame (MovieT *mov, long int frame_num, IplImage **im);


int loadMovie (MovieT **mov, const char *fname);

void releaseMovie (MovieT **mov);

/*does a binary threshold on src and stores result in dst
 *new memory is allocated for dst if dst == NULL or is the wrong size or type (old memory is released)
 **/
void threshold (const IplImage *src, IplImage **dst, double thresh);
/*
 *      mode:
 *  * CV_RETR_EXTERNAL - retrive only the extreme outer contours
    * CV_RETR_LIST - retrieve all the contours and puts them in the list
    * CV_RETR_CCOMP - retrieve all the contours and organizes them into two-level hierarchy: top level are external boundaries of the components, second level are bounda boundaries of the holes
    * CV_RETR_TREE - retrieve all the contours and reconstructs the full hierarchy of nested contours
 **/
CvSeq *findContours (const IplImage *src, int mode, CvMemStorage *storage);
CvPoint *contourToPoints (CvSeq *contour);
void blurIm (const IplImage *src, IplImage *dst, double sigma);
double magnitude (CvPoint v);

double angleBetween (CvPoint v1, CvPoint v2);
//returns x1 - x2;
CvPoint pointDiff (CvPoint x1, CvPoint x2);

CvPoint pointSum (CvPoint x1, CvPoint x2);
CvPoint2D32f pointDiff(CvPoint2D32f p1, CvPoint2D32f p2);
double pointDistance (CvPoint2D32f p1, CvPoint2D32f p2);
int dotProduct (CvPoint v1, CvPoint v2);

int convInd (int ind, int j, int anchor, int npts);
void convolveCircular (double *arr, int arr_npts, double *kernel, int kernel_npts, int anchor, double *out);
void gaussK (double *kernel, double sigma);
void dgaussK (double *kernel, double sigma);
void deriv (double *arr, int arr_npts, double sigma, double *out);
void imInfo (IplImage *src, char *msg);
std::string imStrInfo(IplImage *im);
double crossProduct (CvPoint v1, CvPoint v2);

/*loads the pixels specified by r from the file specified by fname
 *loads as a grayscale image
 *allocates memory in dst if not already there
 */
void loadSubImage (const char *fname, IplImage **dst, CvRect r);

/*copies the subimage specified by r into dst, allocating memory if needed*/
void subImage (IplImage *src, IplImage **dst, CvRect r);

/*copies src into dst, allocating memory if needed*/
void cloneImage (const IplImage *src, IplImage **dst);

/*thresholds the image to a target number of nonzero pixels
  src and dst must be different*/
int thresholdToTargetArea (IplImage *src, IplImage *dst, int targetArea, int tlow, int thigh, int tstart);

int thresholdToNumRegions (IplImage *src, IplImage *dst, int nregions, int minArea, int maxArea, int bestArea);

/*double pointDistance (CvPoint p1, CvPoint p2)
 *
 *distance between p1 and p2
 */
double pointDistance (CvPoint p1, CvPoint p2);
/* CvRect rectIntersect (CvRect r1, CvRect r2)
 *
 * returns the intersection of two rectangles
 * if the rectangles do not intersect, one of length or width will be < 0
 *
 */
CvRect rectIntersect (CvRect r1, CvRect r2);

/*IplImage *LVtoIpl (char* lvsrc, int lvlinewidth, int LVWidth, int LVHeight)
 *
 *wraps a labview image into an IPL image
 */
IplImage *LVtoIpl (char* lvsrc, int lvlinewidth, int LVWidth, int LVHeight);
/*void copyAllocatedImage (IplImage *src, IplImage *dst)
 *
 *copies src to dst making sure the rois match
 *if source roi width > dst roi width (etc.) copies a subset of the src
 *if dst roi wider, copies the entire source into a subset of the dst
 *
 *in all cases, upper left corner of roi1 copies to upper left corner of roi2
 */
void copyAllocatedImage (IplImage *src, IplImage *dst);

double contourArea (CvSeq *contour);

CvPoint2D32f centerOfMass (CvSeq *contour);

/* finds the center of mass, weighted by image intensity within the contour
 * bounding box
 *
 */
CvPoint2D32f centerOfMass (CvSeq *contour, const IplImage *src, double *covMat = NULL);

/*void onlyInContour (IplImage *src, IplImage **dst,CvSeq *contour);
 *returns a copy of src
 *copies only the area within the bounding box of contour, subject to
 *all pixels within the contour are unchanged
 *all pixels outside the contour are 0
 */
void onlyInContour (IplImage *src, IplImage **dst,CvSeq *contour, CvPoint offset);

CvSeq *selectContainingContour (CvSeq *firstContour, CvPoint pt);
CvSeq *selectContainingContour (CvSeq *firstContour, CvPoint2D32f pt);
CvSeq *nearestContour (CvSeq *firstContour, CvPoint pt);
CvSeq *nearestContour (CvSeq *firstContour, CvPoint2D32f pt);

/* finds the contours in src
 * then returns a pointer to the contour containing x0, or failing that
 * a pointer to the contour with center of mass nearest x0, or failing that
 * NULL
 *
 * src is altered by the call
 */

CvSeq *findCorrespondingContour (IplImage *src, CvMemStorage *ms, CvPoint2D32f x0);
CvSeq *findCorrespondingContour (IplImage *src, CvMemStorage *ms, CvPoint x0);
bool inRectangle (CvRect r, CvPoint pt);
bool inRectangle (CvRect r, CvPoint2D32f pt);



std::vector<double> convolveVector (const std::vector<double> &x, const std::vector<double> &kernel, int offset);
std::vector<double> convolveVector (const std::vector<double> &x, const std::vector<double> &kernel);
std::vector<double> gaussKernel (double sigma);
std::vector<double> dgaussKernel (double sigma);
std::vector<double> derivative (const std::vector<double> &x, double sigma);
void simplePlot (const std::vector<double> &x, IplImage **dst);
#endif	
