#ifndef FR_DATA_STORE
#define FR_DATA_STORE


// Forward declarations
class RtData;
class RtDataStore;
class FrMainDocument;

#include "FrMacro.h"
#include "RtOutput.h"
#include <vector>


class FrDataStore : public RtOutput {
public:
    FrDataStore(FrMainDocument* document);
    virtual ~FrDataStore();

    // Overrides
    virtual void setData(RtData *data);

    // Properties
    FrGetPropMacro(RtDataStore*, Store);
    FrPropMacro(FrMainDocument*, Document);

    void AddData(RtData *data);
    void SetStore(RtDataStore* store);

    void GetStuff(std::vector<RtDataID>& data);

private:
    // helpers
    void AddImageToDocument(RtData* data);
    void AddRoiToDocument(RtData* data);

};

#endif
