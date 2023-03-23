#include <cstring>
#include <iostream>
#include "cv.h"
#include "newDLLWrapper.h"
#include "TrackExtractor.h"
#include "MaggotTrackExtractor.h"
#include "BatchExtractor.h"

int analyzeImageStack(
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
                               ) {
    std::ofstream out("c:\\marc_dll_log.txt");

    TrackExtractor *te;
    out << "creating TE" << "\n";
    if (isMaggot) {
        te = new MaggotTrackExtractor();
    } else {
        te = new TrackExtractor();
    }
    if (fstub != NULL) te->fstub = std::string(fstub);
    if (extension != NULL) te->extension = std::string(extension);
    te->padding = padding;
    out << "creating file names\n";
    if (outputname != NULL) {
        te->outputname = std::string(outputname);
        if (!te->outputname.empty()) {
            size_t find1, find2;
            find1 = te->outputname.find('.');
            if (find1 != string::npos) {
                while ((find2 = te->outputname.find('.', find1 +1)) != string::npos) {
                    find1 = find2;
                }
            }
            std::string s;
            if (find1 != string::npos) {
                s = te->outputname.substr(0, find1 - 1);
            } else {
                s = te->outputname;
            }
            s.append("_header.txt");
            te->headerinfoname = s;
        }
    }
    out << "lots of other shit\n";
    if (logname != NULL) te->logName = std::string(logname);
    te->logVerbosity = (verbosityLevelT) verbosityLevel;
    te->startFrame = startFrame;
    te->endFrame = endFrame;
    te->analysisRectangle = cvRect(ar_x0, ar_y0, ar_w, ar_h);
    te->minArea = minArea;
    te->maxArea = maxArea;
    te->overallThreshold = overallThreshold;
    te->winSize = winSize;
    te->nBackgroundFrames = nBackgroundFrames;
    te->background_resampleInterval = background_resample_interval;
    te->background_blur_sigma = background_blur_sigma;
    te->fnm = (_frame_normalization_methodT) frame_norm_method;
    if (thresholdScaleImageName != NULL) te->thresholdScaleImage = std::string(thresholdScaleImageName);
    te->blurThresholdIm_sigma = blurThresholdIm_sigma;
    te->maxExtractDist = maxExtractDist;
    te->showExtraction = showExtraction;
    if (isMaggot) {
        MaggotTrackExtractor *mte = dynamic_cast<MaggotTrackExtractor *> (te);
        if (mte == NULL) {
            out << "dynamic cast failed!\n";
        } else {
            mte->setMaxAngle(maxMaggotContourAngle);
        }
    }
    int rv;
    out << "go\n";
    if ((rv = te->go()) != 0) {
        return rv;
    }
    out << "saveOutput\n";
    if ((rv = te->saveOutput()) != 0) {
        return rv;
    }
    return rv;

}

 TrackExtractor* createTrackExtractor(bool isMaggot) {
    TrackExtractor *te;
    if (isMaggot) {
        te = new MaggotTrackExtractor();
    } else {
        te = new TrackExtractor();
    }
    return te;
}

 unsigned int setExtractorFiles(unsigned int tep,
                                      char *fstub,
                                      char *extension,
                                      int padding,
                                      char *outputname,
                                      char *logname,
                                      int verbosityLevel) {
    TrackExtractor *te = (TrackExtractor *) tep;

    if (fstub != NULL) te->fstub = std::string(fstub);
    if (extension != NULL) te->extension = std::string(extension);
    te->padding = padding;
    //create the header by adding _header.txt to end of output name minus
    //extension
    if (outputname != NULL) {
        te->outputname = std::string(outputname);
        if (!te->outputname.empty()) {
            size_t find1, find2;
            find1 = te->outputname.find('.');
            if (find1 != string::npos) {
                while ((find2 = te->outputname.find('.', find1 +1)) != string::npos) {
                    find1 = find2;
                }
            }
            std::string s;
            if (find1 != string::npos) {
                s = te->outputname.substr(0, find1 - 1);
            } else {
                s = te->outputname;
            }
            s.append("_header.txt");
            te->headerinfoname = s;
        }
    }
    if (logname != NULL) te->logName = std::string(logname);
    te->logVerbosity = (verbosityLevelT) verbosityLevel;
    return (unsigned int) te;
}

 unsigned int setExtractorRanges(unsigned int tep,
                                      int startFrame,
                                      int endFrame,
                                      int ar_x0,
                                      int ar_y0,
                                      int ar_w,
                                      int ar_h) {
    TrackExtractor *te = (TrackExtractor *) tep;
    te->startFrame = startFrame;
    te->endFrame = endFrame;
    te->analysisRectangle = cvRect(ar_x0, ar_y0, ar_w, ar_h);
    return (unsigned int) te;
}

 unsigned int setExtractorLimits(unsigned int tep,
                                      double minArea,
                                      double maxArea,
                                      double maxExtractDist,
                                      double maxMaggotContourAngle,
                                      int winSize) {
    TrackExtractor *te = (TrackExtractor *) tep;
    te->minArea = minArea;
    te->maxArea = maxArea;
    te->winSize = winSize;
    te->maxExtractDist = maxExtractDist;
    MaggotTrackExtractor *mte = dynamic_cast<MaggotTrackExtractor *> (te);
    if (mte != NULL) {
            mte->setMaxAngle(maxMaggotContourAngle);
    }
    return (unsigned int) te;
}

 unsigned int setExtractorThresholdAndBackground(unsigned int tep,
        double overallThreshold,
        int imStackLength,
        int nBackgroundFrames,
        int background_resample_interval,
        double background_blur_sigma,
        int frame_norm_method,
        char *thresholdScaleImageName,
        double blurThresholdIm_sigma){


    TrackExtractor *te = (TrackExtractor *) tep;
    te->overallThreshold = overallThreshold;
    te->imstacklength = imStackLength;
    te->nBackgroundFrames = nBackgroundFrames;
    te->background_resampleInterval = background_resample_interval;
    te->background_blur_sigma = background_blur_sigma;
    te->fnm = (_frame_normalization_methodT) frame_norm_method;
    if (thresholdScaleImageName != NULL) te->thresholdScaleImage = std::string(thresholdScaleImageName);
    te->blurThresholdIm_sigma = blurThresholdIm_sigma;
    return (unsigned int) te;
}

unsigned int setExtractorShowProgress (unsigned int tep,
                                              bool showExtraction){


    TrackExtractor *te = (TrackExtractor *) tep;
    te->showExtraction = showExtraction;
    return (unsigned int) te;
}

unsigned int createBatchExtractor () {
    BatchExtractor *be = new BatchExtractor();
    return (unsigned int) be;
}

unsigned int setDefaultTrackExtractor (unsigned int bep, unsigned int tep) {
    BatchExtractor *be = (BatchExtractor *) bep;
    TrackExtractor *te = (TrackExtractor *) tep;
    be->setDefaultTrackExtractor(te);
    return (unsigned int) be;
}

unsigned int addImageStackToBatchExtractor (unsigned int bep, unsigned int tep, char *fstub, char *outname) {
    BatchExtractor *be = (BatchExtractor *) bep;
    TrackExtractor *te = (TrackExtractor *) tep;
    std::string fs, on;
    if (fstub == NULL) {
        assert (te != NULL);
        fs = te->fstub;
    } else {
        fs = std::string(fstub);
    }
    if (outname == NULL) {
        assert (te != NULL);
        on = te->outputname;
    } else {
        on = std::string(outname);
    }
    be->addImageStack(fs, on, te);
    return (unsigned int) be;
}

unsigned int batchExtractorToYAML (unsigned int bep, char *description_destination, int maxChars) {

    BatchExtractor *be = (BatchExtractor *) bep;
    YAML::Emitter out;
    be->toYaml(out);
    strncpy (description_destination, out.c_str(), maxChars);

    return (unsigned int) be;
}

unsigned int saveBatchExtractorToDisk (unsigned int bep, char *fname){
    BatchExtractor *be = (BatchExtractor *) bep;
    YAML::Emitter out;
    be->toYaml(out);
    ofstream os(fname);
    os << out.c_str();
    std::cout << out.GetLastError() << "\n";
    assert(out.good());
    return (unsigned int) be;
}


unsigned int loadBatchExtractorFromDisk (char *fname) {
    BatchExtractor *be = new BatchExtractor();
    try {
        ifstream is(fname);
        YAML::Parser parser(is);
        YAML::Node node;
        parser.GetNextDocument(node);
        std::cout << "parsed\n";
        be->fromYaml(node);
        
    } catch (YAML::ParserException &e) {
        std::cout << "Parser Error " << e.what() << "\n";
    }
    return (unsigned int) be;
}


unsigned int loadBatchExtractorFromString (const char *str) {
     BatchExtractor *be = new BatchExtractor();
    try {
        std::string s(str);
         istringstream is(s);
          YAML::Parser parser(is);
        YAML::Node node;
        parser.GetNextDocument(node);
        std::cout << "parsed\n";
         be->fromYaml(node);

    } catch (YAML::ParserException &e) {
        std::cout << "Parser Error " << e.what() << "\n";
    }
    return (unsigned int) be;
}

unsigned int runBatchExtractor (unsigned int bep){
    BatchExtractor *be = (BatchExtractor *) bep;
    be->batchProcess();
    return (unsigned int) be;
}

unsigned int runBatchExtractorToMaggot (unsigned int bep, double maxMaggotAngle){
    BatchExtractor *be = (BatchExtractor *) bep;
    be->batchToMaggot(maxMaggotAngle);
    return (unsigned int) be;
}

void clearBatchExtractor (unsigned int bep) {
    BatchExtractor *be = (BatchExtractor *) bep;
    be->setDeleteTEOnDestruction(true);
    delete be;
 }

unsigned int runBatchForeground (unsigned int bep) {
    BatchExtractor *be = (BatchExtractor *) bep;
    be->batchForeground();
    return (unsigned int) be;
}
