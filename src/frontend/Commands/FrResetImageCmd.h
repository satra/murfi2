#ifndef FR_RESET_IMAGE_CMD
#define FR_RESET_IMAGE_CMD

// Forward declarations
class FrLayerSettings;
class FrCameraSettings;
class vtkRenderer;
class vtkImageActor;

// includes
#include "FrMacro.h"
#include "FrBaseCmd.h"

#include <vector>

// This command adjust camera such way that 
// MRI image is visible at the center of view port
// and at the largest scale.
class FrResetImageCmd : public FrBaseCmd {
public:
    enum View { Unknown, Slice, Mosaic, Ortho, Current };
public:
    // Constructor/destructor
    FrResetImageCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrSetPropMacro(View, TargetView);

private:
    // Reset camera
    void ResetCamera(FrCameraSettings* camSets, 
                     vtkImageActor* actor, 
                     vtkRenderer* renderer);

    void ResetTBC(std::vector<FrLayerSettings*>& layers);
};

#endif // FR_RESET_IMAGE_CMD