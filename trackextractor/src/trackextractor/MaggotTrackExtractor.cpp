/* 
 * File:   MaggotTrackExtractor.cpp
 * Author: Marc
 * 
 * Created on November 6, 2009, 3:15 PM
 */

#include "MaggotTrackExtractor.h"
#include "MaggotTrackBuilder.h"
#include "communicator.h"
#include "tictoc.h"
MaggotTrackExtractor::MaggotTrackExtractor() {
    maxAngle = 3.14159253 / 1.99;
}

int MaggotTrackExtractor::go() {
    _TICTOC_TIC_FUNC;
    int rv;
    rv = TrackExtractor::go();
    if (rv != 0 || aborted) {
        _TICTOC_TOC_FUNC;
        return rv;
    }
    if (!outputname.empty()) {
        mh->message("Top Level (Maggot Track Extractor)", verb_warning, " Saving Track Extraction Results to disk before analyzing maggot tracks");
        saveOutput();
    }
    MaggotTrackBuilder *mtb = dynamic_cast<MaggotTrackBuilder *> (tb);
    if (mtb == NULL) {
        mh->message("Top Level (Maggot Track Extractor)", verb_error, " Failed to cast track builder to maggot track builder\n");
        return -1;
    }
    mtb->analyzeMaggotTracks(minArea, maxArea, maxAngle);
    _TICTOC_TOC_FUNC;
    return 0;
}

void MaggotTrackExtractor::createTrackBuilder() {
    tb = new MaggotTrackBuilder(startFrame);
}
YAML::Emitter& MaggotTrackExtractor::yamlBody(YAML::Emitter& out) const {
    out = TrackExtractor::yamlBody(out);
    out << YAML::Key << "max maggot contour angle" << YAML::Value <<  maxAngle;
    return out;
}
void MaggotTrackExtractor::fromYAML(const YAML::Node &node) {
    TrackExtractor::fromYAML(node);
    const YAML::Node *pname = node.FindValue("max maggot contour angle");
    if (pname != NULL) {
        node["max maggot contour angle"] >> maxAngle;
    }

}

YAML::Emitter& operator << (YAML::Emitter& out, const MaggotTrackExtractor *te) {
    std::cout << "out << MTE*\n";
    if (te == NULL) {
        return (out << YAML::Null);
    } else {
        return out << (*te);
    }
}
YAML::Emitter& operator << (YAML::Emitter& out, const MaggotTrackExtractor& te) {
    std::cout << "out << MTE\n";
    return te.toYAML(out);
}
void operator >> (const YAML::Node &node, MaggotTrackExtractor *(&te)) {
    if (YAML::IsNull(node)) {
        te = NULL;
    } else {
        te = new MaggotTrackExtractor();
        te->fromYAML(node);
    }
}

void MaggotTrackExtractor::createPointExtractor() {
    pe = new PointExtractor();
}

void operator >> (const YAML::Node &node, MaggotTrackExtractor &te) {
    if (YAML::IsNull(node)) {
        return;
    } else {
        te.fromYAML(node);
    }
}