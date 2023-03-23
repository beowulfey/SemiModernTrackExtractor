/* 
 * File:   newDLLWrapper.h
 * Author: Marc
 *
 * Created on November 9, 2009, 9:04 AM
 */


#ifndef _NEWDLLWRAPPER_H
#define	_NEWDLLWRAPPER_H

#include "TrackExtractor.h"

#ifndef EXPORT
#ifdef BUILD_DLL
/* DLL export */
#define EXPORT __declspec(dllexport)
#else
/* EXE import */
#define EXPORT __declspec(dllimport)
#endif
#endif

#ifdef	__cplusplus
extern "C" {
#endif


EXPORT int analyzeImageStack(
        char *fstub,
        char *extension,
        int padding,
        char *outputname,
        char *logname,
        int verbosityLevel,
        int startFrame,
        int endFrame,
        int ar_x0,
        int ar_y0,
        int ar_w,
        int ar_h,
        double minArea,
        double maxArea,
        double overallThreshold,
        int winSize,
        int nBackgroundFrames,
        int background_resample_interval,
        double background_blur_sigma,
        int frame_norm_method,
        char *thresholdScaleImageName,
        double blurThresholdIm_sigma,
        double maxExtractDist,
        int showExtraction,
        int isMaggot,
        double maxMaggotContourAngle
                               );

EXPORT TrackExtractor* createTrackExtractor(bool isMaggot);

EXPORT unsigned int setExtractorFiles(unsigned int tep,
                                      char *fstub,
                                      char *extension,
                                      int padding,
                                      char *outputname,
                                      char *logname,
                                      int verbosityLevel);

EXPORT unsigned int setExtractorRanges(unsigned int tep,
                                      int startFrame,
                                      int endFrame,
                                      int ar_x0,
                                      int ar_y0,
                                      int ar_w,
                                      int ar_h);

EXPORT unsigned int setExtractorLimits(unsigned int tep,
                                      double minArea,
                                      double maxArea,
                                      double maxExtractDist,
                                      double maxMaggotContourAngle,
                                      int winSize);

EXPORT unsigned int setExtractorThresholdAndBackground(unsigned int tep,
        double overallThreshold,
        int imStackLength, 
        int nBackgroundFrames,
        int background_resample_interval,
        double background_blur_sigma,
        int frame_norm_method,
        char *thresholdScaleImageName,
        double blurThresholdIm_sigma);


EXPORT unsigned int setExtractorShowProgress (unsigned int tep,
                                              bool showExtraction);

EXPORT unsigned int createBatchExtractor ();

EXPORT unsigned int setDefaultTrackExtractor (unsigned int bep, unsigned int tep);

EXPORT unsigned int addImageStackToBatchExtractor (unsigned int bep, unsigned int tep, char *fstub, char *outname);

EXPORT unsigned int batchExtractorToYAML (unsigned int bep, char *description_destination, int maxChars);

EXPORT unsigned int saveBatchExtractorToDisk (unsigned int bep, char *fname);

EXPORT unsigned int loadBatchExtractorFromDisk (char *fname);

EXPORT unsigned int loadBatchExtractorFromString (const char *str);

EXPORT unsigned int runBatchExtractor (unsigned int bep);

EXPORT unsigned int runBatchExtractorToMaggot (unsigned int bep, double maxMaggotAngle /* = -1 */);

EXPORT void clearBatchExtractor (unsigned int bep);

EXPORT unsigned int runBatchForeground (unsigned int bep);

#ifdef	__cplusplus
}
#endif

#endif	/* _NEWDLLWRAPPER_H */

