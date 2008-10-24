#include "FrCompositeTool.h"
#include "FrMainDocument.h"
#include "FrPanTool.h"
#include "FrTBCTool.h"
#include "FrZoomTool.h"
#include "FrSliceScrollTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

// VTK stuff
#include "vtkRenderer.h"

#include "Qt/QCursor.h"
#include "Qt/QPixmap.h"
#include "Qt/QApplication.h"


FrCompositeTool::FrCompositeTool() {
    m_panTool = new FrPanTool();        
    m_zoomTool = new FrZoomTool();
    m_tbcTool = new FrTBCTool();
	m_ssTool = new FrSliceScrollTool();

	isZoom = false;
	ZoomActivated = false;
}

FrCompositeTool::~FrCompositeTool(){
    delete m_panTool;
    delete m_tbcTool;
    delete m_zoomTool;
	delete m_ssTool;
}

void FrCompositeTool::Start(){
    // Setup controller
    m_panTool->SetController(this->GetController());
    m_tbcTool->SetController(this->GetController());
    m_zoomTool->SetController(this->GetController());
	m_ssTool->SetController(this->GetController());

    m_panTool->Start();
    m_tbcTool->Start();
    m_zoomTool->Start();
	m_ssTool->Start();

    // Update interface to ensure tool is checked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::ManipulationTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(true);
    cmd->Execute();
    delete cmd;
}

void FrCompositeTool::Stop(){

    // Unregister controller
    m_panTool->SetController(0);
    m_tbcTool->SetController(0);
    m_zoomTool->SetController(0);
	m_ssTool->SetController(0);

    m_panTool->Stop();
    m_tbcTool->Stop();
    m_zoomTool->Stop();
	m_ssTool->Stop();

    // Update interface to ensure tool is unchecked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::ManipulationTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(false);
    cmd->Execute();
    delete cmd;
}

bool FrCompositeTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    // Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        if (isZoom)
			m_zoomTool->OnMouseUp(is, params);
		else
			m_panTool->OnMouseUp(is, params);
	}
	else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        m_ssTool->OnMouseUp(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        m_tbcTool->OnMouseUp(is, params);
    }    
    
	ZoomActivated = false;

    return false;
}

bool FrCompositeTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){    
    // here we should check what tool to use: pan or zoom, it depends on mouse coords
	isZoom = CheckMouseParams(is, params);

	// Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
		if (isZoom){
			m_zoomTool->OnMouseDown(is, params);
			ZoomActivated = true;
		}
		else
			m_panTool->OnMouseDown(is, params);
	}
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        m_ssTool->OnMouseDown(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        m_tbcTool->OnMouseDown(is, params);
    }
    return false;
}

bool FrCompositeTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    // here we should check what tool to use: pan or zoom, it depends on mouse coords
	//isZoom = CheckMouseParams(is, params);
	////bool isInViewport = IsInViewPort(is, params);

	//if (!isInViewport){
	//	//change cursor to simple mode
	//	QCursor cursor(Qt::ArrowCursor);
	//	QApplication::setOverrideCursor(cursor);
	//}
	//else if (isZoom){
	//	//change cursor to zoom mode
	//	QCursor cursor(Qt::SizeVerCursor);
	//	QApplication::setOverrideCursor(cursor);
	//}
	//else if (!ZoomActivated){
	//	// change cursor to pan mode
	//	QCursor cursor(Qt::OpenHandCursor);
	//	QApplication::setOverrideCursor(cursor);
	//}

	return false;
}

bool FrCompositeTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    bool result = false;

    // Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        if (isZoom)
			result = m_zoomTool->OnMouseDrag(is, params);
		else
			result = m_panTool->OnMouseDrag(is, params);
	}
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        result = m_ssTool->OnMouseDrag(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        int x = params.X;
        int y = params.Y;
		
		params.Button = FrMouseParams::RightButton;
		result = m_tbcTool->OnMouseDrag(is, params);

        //// First change brightness
        //params.X = x;
        //params.Button = FrMouseParams::RightButton;
        //result = m_tbcTool->OnMouseDrag(is, params);

        //// Then change contrast
        //params.Y = y;
        //params.Button = FrMouseParams::RightButton;
        //result = m_tbcTool->OnMouseDrag(is, params);
    }

    return result;
}

bool FrCompositeTool::CheckMouseParams(FrInteractorStyle* is, FrMouseParams& params){
	// check if coordinates are near left/right border of viewport
	int *size = is->CurrentRenderer->GetSize();
	int *origin = is->CurrentRenderer->GetOrigin();

	int XBorder = size[0];
	int YBorder = size[1];
	int XOrigin = origin[0];
	int YOrigin = origin[0];

    // Leave side only
    int xDelta = size[0] / 6; // delta is about 15%
	if (( (params.X - XOrigin) >= (XBorder-xDelta)) ||
        ( (params.X - XOrigin) <= xDelta)) return true;
	/*if ((params.Y+40 >= YBorder) || (params.Y <= 40)) return true;*/

	return false;
}

bool FrCompositeTool::IsInViewPort(FrInteractorStyle* is, FrMouseParams& params){
	// check if coordinates are near left/right border of viewport
	int *size = is->CurrentRenderer->GetSize();
	int *origin = is->CurrentRenderer->GetOrigin();

	int XBorder = size[0];
	int YBorder = size[1];
	int XOrigin = origin[0];
	int YOrigin = origin[0];

    // Leave side only
	if (( (params.X - XOrigin) == XBorder) ||
        ( (params.X - XOrigin) == 0) ||
		( (params.Y - YOrigin) == YBorder) ||
        ( (params.Y - YOrigin) == 0)) return false;
	
	return true;
}