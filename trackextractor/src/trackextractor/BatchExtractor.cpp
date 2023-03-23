/* 
 * File:   BatchExtractor.cpp
 * Author: Marc
 * 
 * Created on November 12, 2009, 5:00 PM
 */

#include "BatchExtractor.h"
#include "MaggotTrackExtractor.h"
#include "MaggotTrackBuilder.h"

#include <map>


using namespace std;
BatchExtractor::BatchExtractor() {
    deleteTEOnDestruction = true;
    default_te = NULL;
}

BatchExtractor::~BatchExtractor() {
    if (deleteTEOnDestruction) {
        map<string, TrackExtractor*>::iterator sit;
        for (sit = specialParams.begin(); sit != specialParams.end(); ++sit) {
            if (sit->second != NULL) {
                delete sit->second;
            }
        }
        if (default_te != NULL) {
            delete default_te;
        }
    }
}


void BatchExtractor::addImageStack(std::string fstub, std::string outname, TrackExtractor* settings) {
    outnames.insert(pair<string, string>(fstub, outname));
    specialParams.insert(pair<string, TrackExtractor*> (fstub, settings));
    if (settings != NULL) {
        deleteTEOnDestruction = false;
    }
}

//note that whether te is a TrackExtractor or a MaggotTrackExtractor determines
//whether worm or maggot extraction is performed
void BatchExtractor::batchProcess() {
    map<string, string>::iterator nit;
    map<string, TrackExtractor*>::iterator sit;
    cout << "starting batch process of " << outnames.size() << " file sets\n";
    int j = 0;
    for (nit = outnames.begin(); nit != outnames.end(); ++nit) {
        TrackExtractor *te;
        sit = specialParams.find(nit->first);
        if (sit == specialParams.end()) {
            te = NULL;
        } else {
            te = sit->second;
        }
        cout << "processing set #" << ++j << "\n";
        processOne(nit->first, nit->second, te);
    }
}
//if we already have imTracks on disk and want maggot tracks, we can call this
//batch process.  it is not necessary to call it in addition to batch process
void BatchExtractor::batchToMaggot(double maxMaggotAngle) {
    map<string, string>::iterator nit;
    map<string, TrackExtractor*>::iterator sit;
    cout << "starting batch maggot analysis of " << outnames.size() << " file sets\n";
    int j = 0;
    for (nit = outnames.begin(); nit != outnames.end(); ++nit) {
        TrackExtractor *te;
        sit = specialParams.find(nit->first);
        if (sit == specialParams.end()) {
            te = NULL;
        } else {
            te = sit->second;
        }
        cout << "processing set #" << ++j << "\n";
        if (maxMaggotAngle < 0) {
            MaggotTrackExtractor *mte = dynamic_cast<MaggotTrackExtractor *> (te);
            maxMaggotAngle = mte->getMaxAngle();
        }
        processOneToMaggot(nit->first, nit->second, maxMaggotAngle, te);
    }
}

void BatchExtractor::processOne(string fstub, string outname, TrackExtractor* te) {
    if (te == NULL) {
        te = default_te;
    }
    if (te == NULL) {
        cout << "failed to process " << fstub << " to " << outname << " because no trackExtractor or default trackExtractor was provided\n";
        return;
    }
    if (outname.empty()) {
        outname = fstub + "tracks.bin";
    }
    te->fstub = fstub;
    te->outputname = outname;
    string basename = outname;
    size_t ind = basename.find_last_of('.');
    basename.erase(ind);
    te->logName = basename + "_log.txt";
    te->headerinfoname = basename + "_header.txt";
    te->reset();
    cout << "processing " << fstub << "\nto " << outname << "\nlog information is found in " << te->logName << "\nand header info in " << te->headerinfoname << "\n";
    te->go();
    te->saveOutput();
    cout << "finished!\n";
    te->reset();
}

void BatchExtractor::processOneToMaggot(string fstub, string outname, double maxMaggotAngle, TrackExtractor *te) {
    if (te == NULL) {
        te = default_te;
    }
    if (te == NULL) {
        cout << "failed to process " << fstub << " because no trackExtractor or default trackExtractor was provided\n";
        return;
    }
    MaggotTrackBuilder* mtb;
    FILE *f = fopen(outname.c_str(), "rb");
    if (f == NULL) {
        cout << "couldn't open " << outname << "\n";
        return;
    }
    mtb = MaggotTrackBuilder::fromDisk(f);
    if (mtb == NULL) {
        cout << "couldn't load " << outname << "\n";
        return;
    }

    fclose(f);

    //te->fstub = fstub;
    te->outputname = outname;
    string basename = outname;
    size_t ind = basename.find_last_of('.');
    basename.erase(ind);
    te->logName = basename + "_log.txt";
    te->headerinfoname = basename + "_header.txt";

    communicator mh(te->logName.c_str());
    mh.setVerbosity(te->logVerbosity);

    cout << "loaded mtb\n";
    mtb->setMessageHandler(&mh);
    cout << "set mh\n";
    mtb->analyzeMaggotTracks(te->minArea, te->maxArea, maxMaggotAngle);


    cout << "analyzed mtb\n";
   

    te->reset();
    cout << "reset\n";
    te->setTrackBuilder(mtb);
    te->saveOutput();
    cout << "finished!\n";
    te->reset();
}



void BatchExtractor::oneForeground(string fstub, string outname, TrackExtractor* te) {
     if (te == NULL) {
        te = default_te;
    }
    if (te == NULL) {
        cout << "failed to process " << fstub << " because no trackExtractor or default trackExtractor was provided\n";
        return;
    }
   
    te->fstub = fstub;
    te->reset();
    cout << "generating foreground for " << fstub <<"\n";
    if (outname.empty()) {
        outname = fstub + "fore.bmp";
    }
    te->makeForeground(outname);
    te->reset();
}
void BatchExtractor::batchForeground() {
    map<string, string>::iterator nit;
    map<string, TrackExtractor*>::iterator sit;
    cout << "starting batch foreground of " << outnames.size() << " file sets\n";
    int j = 0;
    for (nit = outnames.begin(); nit != outnames.end(); ++nit) {
        TrackExtractor *te;
        sit = specialParams.find(nit->first);
        if (sit == specialParams.end()) {
            te = NULL;
        } else {
            te = sit->second;
        }
        cout << "processing set #" << ++j << "\n";
        //oneForeground(nit->first, nit->second, te);
        oneForeground(nit->first, string(""), te);
//        processOne(nit->first, nit->second, te);
    }
}
YAML::Emitter&BatchExtractor::toYaml(YAML::Emitter& out) {
    map<string, string>::iterator nit;
    map<string, TrackExtractor*>::iterator sit;
    out << YAML::BeginMap;
    out << YAML::Key << "files to process";
    out << YAML::Value << YAML::BeginSeq;
    for (nit = outnames.begin(); nit != outnames.end(); ++nit) {
        TrackExtractor *te;
        sit = specialParams.find(nit->first);
        if (sit == specialParams.end()) {
            te = NULL;
        } else {
            te = sit->second;
        }
        out << YAML::BeginMap;
        out << YAML::Key << "file stub" << YAML::Value << nit->first;
        out << YAML::Key << "output file" << YAML::Value << nit->second;
        out << YAML::Key << "processing params" << YAML::Value;
        
        if (te == NULL) {
            out << YAML::Null;
        } else {
            te->toYAML(out);
        }

        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    out << YAML::Key << "default processing params" << YAML::Value;
    
    if (default_te == NULL) {
        out << YAML::Null;
    } else {
        default_te->toYAML(out);
    }

    out << YAML::EndMap;
    return out;
}

void BatchExtractor::fromYaml(const YAML::Node& node) {
    try {
         if (node.GetType() == YAML::CT_NONE) {
            return;
        }
        if (node.FindValue("default processing params") == NULL) {
            return;
        }
        const YAML::Node& dfs = node["default processing params"];
        if (dfs.GetType() != YAML::CT_NONE &&  dfs.FindValue("max maggot contour angle")) { //it's a maggot
            MaggotTrackExtractor *mte;
            dfs >> mte;
            default_te = mte;
        } else {
            dfs >> default_te;
        }
        const YAML::Node& imstack = node["files to process"];
   
        string fstub, outname;
        TrackExtractor *settings;
        for (unsigned i = 0; i < imstack.size(); ++i) {
            imstack[i]["file stub"] >> fstub;
            imstack[i]["output file"] >> outname;
           // imstack[i]["processing params"] >> settings;
            const YAML::Node& set = imstack[i]["processing params"];
            if (set.FindValue("max maggot contour angle")) { //it's a maggot
                MaggotTrackExtractor *mte;
                set >> mte;
                settings = mte;
            } else {
                 set >> settings;
            }
            outnames.insert(pair<string, string>(fstub, outname));
            specialParams.insert(pair<string, TrackExtractor*> (fstub, settings));
        }
    } catch (YAML::ParserException& e) {
        cout << "batch extractor load failed with exception: " << e.what() << "\n";
    }

}


BatchExtractor::BatchExtractor(const BatchExtractor& orig) {

}

