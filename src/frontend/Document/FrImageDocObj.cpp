#include "FrImageDocObj.h"
#include "FrDocument.h"
#include "FrLayerDocObj.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Backend includes
#include "RtMRIImage.h"
#include "RtDataID.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>

#define BAD_SERIES_NUM 0xffffffff

FrImageDocObj::FrImageDocObj(){
    m_Images.clear();
    m_SeriesNumber = BAD_SERIES_NUM;
}

FrImageDocObj::FrImageDocObj(RtMRIImage* img){    
    m_Images.push_back(img);
    m_SeriesNumber = img->getDataID().getSeriesNum();
}

FrImageDocObj::~FrImageDocObj(){
    this->ClearAll();
}

void FrImageDocObj::OnAdd(FrDocument* doc){
    //NOTE: Since there may be th only one instance of image layer
    //image layer creation and deletetion in TabSettingsDocObj

    //// Add here Image layer
    //FrLayerDocObj* layerDO = new FrLayerDocObj(FrLayerSettings::LImage);
    //layerDO->SetID(DEF_LAYER_ID);
    //doc->Add(layerDO);  
}

void FrImageDocObj::OnRemove(FrDocument* doc){
    //NOTE: Since there may be th only one instance of image layer
    //image layer creation and deletetion in TabSettingsDocObj

    //// Remove here image layer
    //std::vector<FrDocumentObj*> objects;
    //
    //doc->GetObjectsByType(objects, FrDocumentObj::LayerObject);
    //std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());

    //for(it = objects.begin(); it != itEnd; ++it){
    //    FrLayerDocObj* layerDO = (FrLayerDocObj*)(*it);

    //    // Image layer is the only one and has predefined ID
    //    if(layerDO->GetID() == DEF_LAYER_ID){
    //        doc->Remove( (*it) );
    //        break;
    //    }
    //}
}

FrDocumentObj::ObjTypes FrImageDocObj::GetType(){
    return FrDocumentObj::ImageObject;
}

RtMRIImage* FrImageDocObj::GetTimePointData(unsigned int timePoint){
    RtMRIImage* result = 0;

    ImageCollection::iterator it, itEnd(m_Images.end());
    for(it = m_Images.begin(); it != itEnd; ++it){
        RtMRIImage* img = (RtMRIImage*)(*it);
  //      if(img->getDataID().getTimePoint() == timePoint){
            result = img;
            break;
  //      }
    }

    return result;
}

bool FrImageDocObj::AddTimePointData(RtMRIImage* mriImage){
    // Some checks
    if(!mriImage) return false;
    if(m_SeriesNumber == BAD_SERIES_NUM){
        m_SeriesNumber = mriImage->getDataID().getSeriesNum();
    }

    // Add only with the same series number
    if(m_SeriesNumber != mriImage->getDataID().getSeriesNum()){
        return false;
    }

    // Check if we have this timepoint
    bool hasTP = false;
    unsigned int timePoint = mriImage->getDataID().getTimePoint();
    ImageCollection::iterator it, itEnd(m_Images.end());
    for(it = m_Images.begin(); (it != itEnd) && !hasTP; ++it){
        RtMRIImage* img = (RtMRIImage*)(*it);
        hasTP = (img->getDataID().getTimePoint() == timePoint);
    }

    if(hasTP) return false;
    m_Images.push_back(mriImage);

    return false;
}

void FrImageDocObj::ClearAll(){
    ImageCollection::iterator it, itEnd(m_Images.end());
    for(it = m_Images.begin(); it != itEnd; ++it){
        delete (*it);
    }
    m_Images.clear();
    m_SeriesNumber = BAD_SERIES_NUM;
}
