/* 
 * File:   BatchExtractor.h
 * Author: Marc
 *
 * Created on November 12, 2009, 5:00 PM
 */

#ifndef _BATCHEXTRACTOR_H
#define	_BATCHEXTRACTOR_H

#include <cstring>
#include <vector>
#include <map>
#include "yaml.h"
#include "TrackExtractor.h"


class BatchExtractor {
public:
    BatchExtractor();
   
    virtual ~BatchExtractor();
    YAML::Emitter &toYaml (YAML::Emitter& out);
    void fromYaml(const YAML::Node& node);

    void addImageStack (std::string fstub, std::string outname, TrackExtractor *settings = NULL);
    inline void setDefaultTrackExtractor (TrackExtractor *te) {
        this->default_te = te;
        deleteTEOnDestruction = false;
    }
    inline TrackExtractor *getDefaultTrackExtractor() {
        return default_te;
    }

    void batchProcess();

    void processOne (string fstub, string outname, TrackExtractor *te);

    void batchForeground();
    void oneForeground(string fstub, string outname, TrackExtractor *te);

    void batchToMaggot(double maxMaggotAngle);

    void processOneToMaggot(string fstub, string outname, double maxMaggotAngle, TrackExtractor *te);
    
    inline void setDeleteTEOnDestruction (bool d) {
        deleteTEOnDestruction = d;
    }
protected:
    std::map<std::string,std::string>  outnames;
    std::map<std::string,TrackExtractor *> specialParams;
    TrackExtractor *default_te;
    bool deleteTEOnDestruction;

private:
     BatchExtractor(const BatchExtractor& orig);
};

#endif	/* _BATCHEXTRACTOR_H */

