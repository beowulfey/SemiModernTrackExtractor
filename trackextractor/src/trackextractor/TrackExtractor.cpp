/* 
 * File:   TrackExtractor.cpp
 * Author: Marc
 * 
 * Created on November 6, 2009, 10:48 AM
 */
#include <cstring>
#include <ctime>
#include <sstream>
#include "trackbuilder.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "TrackExtractor.h"
#include "PointExtractor.h"
#include "communicator.h"
#include "ImLoader.h"
#include "CvUtilsPlusPlus.h"
#include "WormPointExtractor.h"
#include "tictoc.h"
using std::string;

static const int update_every_secs = 30;
TrackExtractor::TrackExtractor() {
     /*****************************
     *  FILE INPUT/OUTPUT PARAMS *
     *****************************/

    logVerbosity = verb_warning; //set to off for no messages out


    /**********************************
     *  REGION EXTRACTION PARAMS      *
     **********************************/

    blurThresholdIm_sigma = 0;

    startFrame = 0;
    endFrame = 1000;
    analysisRectangle = cvRect(0,0,-1,-1);
    winSize = -1; //size of image to save with every track Point; -1 for off
    nBackgroundFrames = 5;
    background_resampleInterval = 200;
    background_blur_sigma = 1;

    fnm = _frame_none;

    imstacklength = 1;
    /****************************************
     *      TRACK STITCHING PARAMS          *
     ****************************************/

    maxExtractDist = 7;
    minArea = 10;
    maxArea = 500;

    /****************************************
     *      OPTIONAL PARAMS                 *
     ****************************************/
    showExtraction = false; //whether to show the points as they are extracted; slows stuff down
    tb = NULL;
    pe = NULL;
    mh = NULL;

}


TrackExtractor::~TrackExtractor() {
    reset();
}

void TrackExtractor::reset() {
 //  cout << "delete tb\n";
    if (tb != NULL) {
        tb->deleteAllTracks();
        delete tb;
    }
//    cout << "delete pe\n";
    if (pe != NULL) {
        delete pe;
    }
 //   cout << "delete mh\n";
    if (mh != NULL) {
        delete mh;
    }
 //   cout << "done deleting, setting all to null\n";
    tb = NULL;
    pe = NULL;
    mh = NULL;
}
void TrackExtractor::ready() {
    _TICTOC_TIC_FUNC;
     if (logName.empty()) {
        mh = new communicator();
    } else {
        mh = new communicator(logName.c_str());
    }
    mh->setVerbosity(logVerbosity);
  //  cout << "mh created\n";
    startMessage();
    //tb = new TrackBuilder(startFrame);
    createTrackBuilder();
    tb->setMessageHandler(mh);
  //  cout << "track builder created\n";
   // pe = new PointExtractor ();
    createPointExtractor();
    pe->setMessageHandler(mh);
  //  cout << "pe created\n";

    pe->setWinSize(winSize);
    pe->setAnalysisRect(analysisRectangle);
    pe->setFrameRange(startFrame, endFrame);
    pe->setBackgroundParams(nBackgroundFrames, background_resampleInterval, background_blur_sigma);
    pe->setAreaRange(minArea, maxArea);
    pe->setFrameNormalizationMethod(fnm);
    ImLoader *fl = new ImLoader();
    fl->setFileName(fstub.c_str(), extension.c_str());
    fl->setAnalysisRect(analysisRectangle);
    fl->setMessageHandler(mh);
    fl->setPadding(padding);
    pe->setFrameLoader(fl);

    tb->setPointExtractor(pe);
    tb->setConnectionDistance(maxExtractDist);

  //  cout << "frame loader created and set\n";
    getThresholdImage();
    _TICTOC_TOC_FUNC;
 //  cout << "ready() done\n";
}
int TrackExtractor::go() {
    _TICTOC_TIC_FUNC;
    ready();
    IplImage *im = NULL;
    mh->message("Track Extractor ", verb_message, "creating named window\n");
    int abort = 0; aborted = false;
    if (showExtraction) {
        cvDestroyAllWindows();
        cvNamedWindow("Extraction Progress", 0);
        cvCreateTrackbar("click to abort", "Extraction Progress", &abort, 1, NULL);
    }
    starttime = clock();
    clock_t lasttime = starttime;
    mh->message("Track Extractor ", verb_message, "starting extraction\n");
    int countdown = 1;
    bool update = true;
    while (tb->completeStep() == 0) {

        
        if (showExtraction) {
            //only update the image every so often, instead of every time;
            //we set it so that the average time per frame is max 50 ms
            if (update && (--countdown) <= 0) {
                clock_t t1 = clock();
                tb->extractionImage(&im);
                clock_t extraction_time = clock() - t1;
                countdown = extraction_time / 50 + 1;
            }

            cvShowImage("Extraction Progress", im);
            if (abort) {
                aborted = true;
                break;
            }
            char key = toupper(cvWaitKey(1));
            if ( key == 'Q') {
                aborted = true;
                break;
            };
            if (key == 'V') {
                update = !update;
            }
        }
        if ((clock() - lasttime) > update_every_secs*CLOCKS_PER_SEC) {
            cout << (pe->getFrameNumber() - startFrame) << " frames extracted. " << (clock()-starttime) / (pe->getFrameNumber() - startFrame) << " ms per frame.\n";
            lasttime = clock();
        }
    }
    mh->message("Track Extractor", verb_error, TICTOC::timer().generateReportCstr());
    cvDestroyAllWindows();
     _TICTOC_TOC_FUNC;
    return 0;
   
}

void TrackExtractor::createPointExtractor() {
    if (imstacklength <= 1) {
        pe = new PointExtractor();
    } else {
        WormPointExtractor *wpe = new WormPointExtractor();
        wpe->setDilationDist(this->maxExtractDist/2);
        wpe->setStackLength(imstacklength);
        pe = wpe;
    }
}

void TrackExtractor::createTrackBuilder() {
    tb = new TrackBuilder(startFrame);
}

int TrackExtractor::saveOutput() {
    if (aborted) {
        return -1;
    }
    FILE *f = fopen (outputname.c_str(), "wb");

    if (f == NULL) {
        cout << "failed to open file " << outputname;
        return -1;
    } else if (tb->toDisk(f) == 0) {
        cout << "tracks saved to disk!\n";
    } else {
        cout << "write failed\n";
        fclose(f);
        return -1;
    }
    fclose (f);
    ofstream os(headerinfoname.c_str());
    string fgname = fstub + "foreground." + extension;
    if (os.is_open()) {
        YAML::Emitter out;
        toYAML(out);
        os << out.c_str() << "\n\n";
        os << outputname << "\ncontains the analysis of " <<"\n" << fstub << "\nstored in the following format:\n";
        os << tb->saveDescription();
        if (pe != NULL) {
            os << "\n\nWriting autogenerated foreground image to " << fgname << "\n";
        }
        os.close();
    }
    IplImage *fg = NULL;
    if (pe != NULL) {
        pe->copyOutAutoGeneratedForeground(&fg);
        cvSaveImage(fgname.c_str(), fg);
        cvReleaseImage(&fg);
    }
    return 0;
}
int TrackExtractor::getThresholdImage() {
    //set the threshold image globally, so that the point extractor knows about the
    //maximum threshold
    pe->setGlobalThreshold(overallThreshold);
    if (thresholdScaleImage.empty()) {
        return 1;
    }
    IplImage *im = NULL, *foreground = NULL, *foreIm = NULL;
    im = cvLoadImage(thresholdScaleImage.c_str(), 0);
    if (im == NULL) {
        thresholdScaleImage = string("failed to load threshold image ") + thresholdScaleImage;
        mh->message ("Track Extractor", verb_warning, thresholdScaleImage.c_str());
        pe->setGlobalThreshold(overallThreshold);
        return -1;
    }
    allocateImage(&foreground, cvGetSize(im), IPL_DEPTH_8U, 1);
    cvConvertScale(im, foreground, overallThreshold/255.0, 0);
    if (blurThresholdIm_sigma > 0) {
        blurIm(foreground, foreground, blurThresholdIm_sigma);
    }
    subImage(foreground, &foreIm, analysisRectangle);
    pe->setThresholdCompareIm(foreIm);

    cvReleaseImage(&im);
    cvReleaseImage(&foreground);
    cvReleaseImage(&foreIm);
    mh->message ("Track Extractor", verb_message, "loaded threshold compare image\n");
    return 0;
}

YAML::Emitter& TrackExtractor::toYAML(YAML::Emitter& out) const {
    out << YAML::BeginMap;
    out = yamlBody(out);
   // std::cout << "Emitter error: " << out.GetLastError() << "\n";
    out << YAML::EndMap;
    return out;
}

YAML::Emitter& TrackExtractor::yamlBody(YAML::Emitter& out) const {

    /********************************************************************
     *       EXTRACTION PARAMETERS (PUBLIC DATA)                        *
     ********************************************************************/

     /*****************************
     *  FILE INPUT/OUTPUT PARAMS *
     *****************************/

    out << YAML::Key << "fstub" << YAML::Value << fstub; //input
    out << YAML::Key << "extension" << YAML::Value << extension; //input extension; //input
    out << YAML::Key << "padding" << YAML::Value << padding;
    out << YAML::Key << "outputname" << YAML::Value <<  outputname; //output
    out << YAML::Key << "headerinfoname" << YAML::Value <<  headerinfoname; //output
    out << YAML::Key << "logName" << YAML::Value <<  logName; //if empty, log goes to cout
    out << YAML::Key << "verbosity level" << YAML::Value <<  (int) logVerbosity; //set to off for no messages out


    /**********************************
     *  REGION EXTRACTION PARAMS      *
     **********************************/


    out << YAML::Key << "startFrame" << YAML::Value <<  startFrame;
    out << YAML::Key << "endFrame" << YAML::Value <<  endFrame;
    out << YAML::Key << "analysis rectangle" << YAML::Value <<  YAML::BeginSeq << analysisRectangle.x << analysisRectangle.y << analysisRectangle.width << analysisRectangle.height << YAML::EndSeq;

    out << YAML::Key << "minArea" << YAML::Value <<  minArea;
    out << YAML::Key << "maxArea" << YAML::Value <<  maxArea;
    out << YAML::Key << "overallThreshold" << YAML::Value <<  overallThreshold; //if thresholdScaleImage is empty, this is the global threshold

    out << YAML::Key << "winSize" << YAML::Value <<  winSize; //size of image to save with every track Point; -1 for off
    out << YAML::Key << "nBackgroundFrames" << YAML::Value <<  nBackgroundFrames;
    out << YAML::Key << "background_resampleInterval" << YAML::Value <<  background_resampleInterval;
    out << YAML::Key << "background_blur_sigma" << YAML::Value <<  background_blur_sigma;

    //thresholdScaleImage is a grayscale image with values 0 to 255
    //if t is the overall threshold, then the thresholdCompareImage is created by
    //t * thresholdScaleImage / 255;
     out << YAML::Key << "thresholdScaleImage" << YAML::Value <<  thresholdScaleImage;


    out << YAML::Key << "blurThresholdIm_sigma" << YAML::Value <<  blurThresholdIm_sigma;

    out << YAML::Key << "frame normalization method" << YAML::Value << fnm;

    out << YAML::Key << "imStackLength" << YAML::Value << imstacklength;
    /****************************************
     *      TRACK STITCHING PARAMS          *
     ****************************************/

    out << YAML::Key << "maxExtractDist" << YAML::Value <<  maxExtractDist;

    /****************************************
     *      OPTIONAL PARAMS                 *
     ****************************************/
    out << YAML::Key << "showExtraction" << YAML::Value <<  showExtraction; //whether to show the points as they are extracted; slows stuff down
    return out;
}

void TrackExtractor::fromYAML(const YAML::Node& node) {

    /********************************************************************
     *       EXTRACTION PARAMETERS (PUBLIC DATA)                        *
     ********************************************************************/

     /*****************************
     *  FILE INPUT/OUTPUT PARAMS *
     *****************************/

    node["fstub"] >> fstub; //input
    node["extension"] >> extension; //input extension; //input
    node["padding"] >> padding;
    node["outputname"] >> outputname; //output
    node["headerinfoname"] >> headerinfoname; //output
    node["logName"] >> logName; //if empty, log goes to cout
    int vl;
    node["verbosity level"] >> vl;
    logVerbosity = (verbosityLevelT) vl; //set to off for no messages out


    /**********************************
     *  REGION EXTRACTION PARAMS      *
     **********************************/


    node["startFrame"] >> startFrame;
    node["endFrame"] >> endFrame;

    node["analysis rectangle"][0] >> analysisRectangle.x;
    node["analysis rectangle"][1] >> analysisRectangle.y;
    node["analysis rectangle"][2] >> analysisRectangle.width;
    node["analysis rectangle"][3] >> analysisRectangle.height;

    node["minArea"] >> minArea;
    node["maxArea"] >> maxArea;
    node["overallThreshold"] >> overallThreshold; //if thresholdScaleImage is empty, this is the global threshold

    node["winSize"] >> winSize; //size of image to save with every track Point; -1 for off
    node["nBackgroundFrames"] >> nBackgroundFrames;
    node["background_resampleInterval"] >> background_resampleInterval;
    node["background_blur_sigma"] >> background_blur_sigma;
    int fnmeth;
    node["frame normalization method"] >> fnmeth;
    fnm = (_frame_normalization_methodT) fnmeth;

    if(const YAML::Node *pisl = node.FindValue("imStackLength")) {
        *pisl >> imstacklength;
    } else {
        if (mh != NULL) {
            mh->message("TrackExtractor loading", verb_warning, "imStackLength not found");
        } else {
            std::cout << "imStackLength not found";
        }
        imstacklength = 1;
    }
    //thresholdScaleImage is a grayscale image with values 0 to 255
    //if t is the overall threshold, then the thresholdCompareImage is created by
    //t * thresholdScaleImage / 255;
     node["thresholdScaleImage"] >> thresholdScaleImage;


    node["blurThresholdIm_sigma"] >> blurThresholdIm_sigma;
    /****************************************
     *      TRACK STITCHING PARAMS          *
     ****************************************/

    node["maxExtractDist"] >> maxExtractDist;

    /****************************************
     *      OPTIONAL PARAMS                 *
     ****************************************/
    node["showExtraction"] >> showExtraction; //whether to show the points as they are extracted; slows stuff down

}

void TrackExtractor::startMessage() {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    YAML::Emitter out;
    toYAML(out);
    mh->setVerbosity(verb_debug);
    mh->message("Extraction Parameters", verb_message, out.c_str());
    stringstream s("Beginning extraction at "); s << asctime (timeinfo);
    mh->message("Track Extractor", verb_message, s.str().c_str());
    mh->setVerbosity(logVerbosity);
}
void TrackExtractor::endMessage() {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    stringstream s("Finished extraction at "); s << asctime (timeinfo) << " ";
    s << (endFrame - startFrame) / difftime(rawtime, starttime) << " frames per second\n";
    mh->setVerbosity(verb_debug);
    mh->message("Track Extractor", verb_message, s.str().c_str());
    mh->setVerbosity(logVerbosity);
}
YAML::Emitter& operator << (YAML::Emitter& out, const TrackExtractor *te) {
    std::cout << "emitter << TrackExtractor*\n";
    if (te == NULL) {
        return (out << YAML::Null);
    } else {
        return out << *te;
    }
}
YAML::Emitter& operator << (YAML::Emitter& out, const TrackExtractor &te) {
    std::cout << "emitter << TrackExtractor\n";
    return te.toYAML(out);
}
void operator >> (const YAML::Node &node, TrackExtractor * &te) {
    if (YAML::IsNull(node)) {
        te = NULL;
    } else {
        te = new TrackExtractor();
        te->fromYAML(node);
    }
}

void operator >> (const YAML::Node &node, TrackExtractor &te) {
    if (YAML::IsNull(node)) {
        return;
    } else {
        te.fromYAML(node);
    }
}

void TrackExtractor::makeForeground(string outname) {
//    string outname = fstub + "fore.bmp";
    ready();
    IplImage *fg = pe->generateForeground(10);
    cout << "foreground generated\n";
    cout << "trying to save:\n" << outname.c_str() << "\n";
    cvSaveImage(outname.c_str(), fg);

    cvReleaseImage(&fg);
}