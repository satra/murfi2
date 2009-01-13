#include "FrGraphCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrUtils.h"
#include "FrRoiDocObj.h"
#include "FrLayerSettings.h"
#include "FrGraphSettings.h"
#include "FrLayerDocObj.h"
#include "FrViewDocObj.h"
#include "FrQwtPlotWidget.h"
#include "FrGraphDocObj.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"

#include "Qt/qmessagebox.h"


#define ALL_ITEMS_COUNT 5

FrGraphCmd::FrGraphCmd() 
: m_Action(FrGraphCmd::Undefined), 
  m_DocObj(0), m_isID(false){
}

void FrGraphCmd::SetID(int id){
    m_ID = id;
    m_isID = true;
}

bool FrGraphCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
        case FrGraphCmd::Add: result = AddGraph();
            break;
        case FrGraphCmd::Delete: result = DeleteGraph();
            break;
        case FrGraphCmd::Undefined:
        default:
            // Do nothing here
            break;
    }
    return result;
}

bool FrGraphCmd::AddGraph(){
    if(m_DocObj == 0) return false;
    
    //// add graph to FrQwtPlotWidget
    //FrMainWindow* mv = this->GetMainController()->GetMainView();
    //
    //FrGraphSettings* gs;
    //gs = m_DocObj->GetSettings();
    //mv->GetGraphPaneWidget()->GetQwtPlotWidget()->AddGraph(m_DocObj->GetID(), gs->Name, gs->Color);

    return true;
}

bool FrGraphCmd::DeleteGraph(){
    if(m_DocObj == 0) return false;

    //// remove graph from FrQwtPlotWidget
    //FrMainWindow* mv = this->GetMainController()->GetMainView();
    //
    //FrGraphSettings* gs;
    //gs = m_DocObj->GetSettings();
    //mv->GetGraphPaneWidget()->GetQwtPlotWidget()->RemoveGraph(m_DocObj->GetID());

    return true;
}

// delete active layer
int FrGraphCmd::GetActiveLayerID(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    return viewDO->GetActiveLayerID();
}

bool FrGraphCmd::IsRoiLayer(int id){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    
    std::vector<FrDocumentObj*> objects;
    doc->GetObjectsByType(objects, FrDocumentObj::RoiObject);

    // Iterate through all found rois
    std::vector<FrDocumentObj*>::iterator itr, itrEnd(objects.end());
    for(itr = objects.begin(); itr != itrEnd; ++itr){
        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*itr);
        if(id == roiDO->GetID()) return true;
    }
    return false;
}
///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrGraphCmd::CanUndo(){
    return false;
}

bool FrGraphCmd::Undo(){
    return false;
}

bool FrGraphCmd::Redo(){
    return false;
}