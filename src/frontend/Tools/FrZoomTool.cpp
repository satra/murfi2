#include "FrZoomTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "Qt/QApplication.h"
#include "Qt/QCursor.h"

#define DEFAULT_ZOOM_FACTOR 10.0
#define DEFAULT_BASE 1.1

FrZoomTool::FrZoomTool()
: m_oldX(0), m_oldY(0), 
  m_zoomFactor(DEFAULT_ZOOM_FACTOR){
}

FrZoomTool::~FrZoomTool(){
}

void FrZoomTool::Start(){
}

void FrZoomTool::Stop(){
}

bool FrZoomTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
//    is->CurrentRenderer->GetRenderWindow()->SetCurrentCursor(VTK_CURSOR_DEFAULT);
	QCursor cursor(Qt::ArrowCursor);
	QApplication::setOverrideCursor(cursor);   
    return false;
}

bool FrZoomTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        m_oldX = params.X;
        m_oldY = params.Y;

        //is->CurrentRenderer->GetRenderWindow()->SetCurrentCursor(VTK_CURSOR_SIZENS);
		QCursor cursor(Qt::SizeVerCursor);
		QApplication::setOverrideCursor(cursor);
    }    
    return false;
}

bool FrZoomTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrZoomTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){

    if (params.Button != FrMouseParams::LeftButton ||
        is->CurrentRenderer == NULL) return false;
        
    double deltaY = params.Y - m_oldY;
    m_oldX = params.X;
    m_oldY = params.Y;

    double centerY = is->CurrentRenderer->GetCenter()[1];
    double dyFactor = this->m_zoomFactor * deltaY / centerY;

    this->ZoomByScaling(pow(DEFAULT_BASE, dyFactor), is);

    return true;
}

void FrZoomTool::ZoomByScaling(double factor, FrInteractorStyle* is){
  
    vtkCamera *camera = is->CurrentRenderer->GetActiveCamera();
    
	double curScale = camera->GetParallelScale() / factor;
	if (curScale > MAX_ZOOM) curScale = MAX_ZOOM;
	if (curScale < MIN_ZOOM) curScale = MIN_ZOOM;
        
	if (camera->GetParallelProjection()) {
        FrChangeCamCmd* cmd = FrCommandController::CreateCmd<FrChangeCamCmd>();
        cmd->SetMouseXY(m_oldX, m_oldY);
        cmd->SetScale(curScale);
        cmd->Execute();
    }
    else{
        // For perspective projection do not support command yet
        camera->Dolly(factor);
        if (is->AutoAdjustCameraClippingRange) {
            is->CurrentRenderer->ResetCameraClippingRange();
        }
    }
}