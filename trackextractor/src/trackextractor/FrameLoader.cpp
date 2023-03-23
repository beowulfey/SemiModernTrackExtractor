/* 
 * File:   FrameLoader.cpp
 * Author: Marc
 * 
 * Created on October 22, 2009, 1:42 PM
 */

#include <cstring>
#include <sstream>
#include "FrameLoader.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "CvUtilsPlusPlus.h"
#include "tictoc.h"

FrameLoader::FrameLoader() {
    Init();
}



FrameLoader::~FrameLoader() {
  //  void *ptrsToFree[] = {fileName, extension, loadIm, convertedIm};
    if (fileName != NULL) free(fileName);
    if (extension != NULL) free(extension);
    if (loadIm != NULL) cvReleaseImage(&loadIm);
    if (convertedIm != NULL) cvReleaseImage(&convertedIm);
  
}

void FrameLoader::Init() {
    void **ptrsToInit[] = {(void **) &fileName, (void **) &extension, (void **) &loadIm, (void **) &convertedIm};
    int n = 4;
    for (int j = 0; j < n; ++j) {
        *(ptrsToInit[j]) = NULL;
    }
    mh = NULL;
    ar.width = -1;
    ar.height = -1;
    ar.x = 0;
    ar.y = 0;
    lastFrameLoaded = -10000;
}

void FrameLoader::setFileName (const char *fileName, const char *extension) {
    if (this->fileName != NULL)
        free (this->fileName);
    this->fileName = (char *) malloc (strlen(fileName) + 1);
    strcpy (this->fileName, fileName);
    if (this->extension != NULL)
        free (this->extension);
    this->extension = (char *) malloc (strlen(extension) + 1);
    strcpy (this->extension, extension);
}


void FrameLoader::setAnalysisRect (CvRect r) {

    ar = r;
    stringstream s;
    s << "analysis rectangle set to " << r.x << " " << r.y << " " << r.width << " " << r.height;
    message(s.str().c_str(), verb_message);
}

int FrameLoader::getFrame(int frameNumber, IplImage** dst) {
    return getFrame(frameNumber, dst, _frame_none, 0);
}

int FrameLoader::getFrame(int frameNumber, IplImage **dst, _frame_normalization_methodT fnm, double normTarget) {
    _TICTOC_TIC_FUNC;
    message ("getFrame called ", verb_debug);
    if (lastFrameLoaded != frameNumber) {
        if (loadWholeFrame(frameNumber) != 0){
            _TICTOC_TOC_FUNC;
            return -1;
        };
        lastFrameLoaded = frameNumber;
    }
    if (loadIm == NULL) {
        stringstream s("failed to load frame ");
        s << frameNumber << "\n";
        message (s.str().c_str(), verb_error);
        _TICTOC_TOC_FUNC;
        return -1;
    }
    checkAr();
    allocateImage(&convertedIm, loadIm);
    if (fnm == _frame_none) {
        cloneImage(loadIm, &convertedIm);
    } else {
        if (normTarget <= 0) {
            message("You asked me to normalize an image, but gave me a target <= 0", verb_warning);
            cloneImage(loadIm, &convertedIm);
        } else {
            double nf = getFrameNormFactor(frameNumber, fnm);
            if (nf > 0) {
                stringstream s; s << ("scaling frame: target norm factor = ");
                s << normTarget << " and this frame has nf: " << nf;
                nf = normTarget / nf;
                s << " so I am multiplying by " << nf << "\n";
                cvConvertScale(loadIm, convertedIm, nf, 0);
                message(s.str().c_str(), verb_verbose);
            } else {
                message("Norm factor returned a value <= 0", verb_warning);
                cloneImage(loadIm, &convertedIm);
            }
        }
    }
    subImage (convertedIm, dst, ar);
    _TICTOC_TOC_FUNC;
    return 0;
}

double FrameLoader::getFrameNormFactor (int frameNumber,  _frame_normalization_methodT fnm) {
    _TICTOC_TIC_FUNC;
    if (lastFrameLoaded != frameNumber) {
        if (loadWholeFrame(frameNumber) != 0){
            _TICTOC_TOC_FUNC;
            return -1;
        };
        lastFrameLoaded = frameNumber;
    }
    switch (fnm){
        case _frame_none:
            _TICTOC_TOC_FUNC;
            return 0;
            break;
        case _frame_wholeImage:
            _TICTOC_TOC_FUNC;
            return (int) (cvSum(loadIm).val[0]);
            break;
        case _frame_excerptedRect:
            checkAr();
            CvRect roi = cvGetImageROI(loadIm);
            cvSetImageROI(loadIm, ar);
            int rv = (int) (cvSum(loadIm).val[0]);
            cvSetImageROI(loadIm, roi);
            _TICTOC_TOC_FUNC;
            return rv;
            break;
    }
    _TICTOC_TOC_FUNC;
    return 0; //should never reach this point
}

void FrameLoader::checkAr() {
    if (ar.width < 0 || ar.height < 0) {
        message ("analysis rectangle was not set", verb_warning);
        ar.x = 0;
        ar.y = 0;
        if (loadIm == NULL) {
            message ("loadIm is NULL.", verb_error);
        } else {
            ar.width = loadIm->width;
            ar.height = loadIm->height;
        }
    }
}

