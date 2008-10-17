#ifndef FR_MAIN_CONTROLLER
#define FR_MAIN_CONTROLLER

#include "FrController.h"

// Forward declaration
class FrTool;
class FrMainWindow;
class FrMainDocument;
class FrToolController;
class FrInteractorStyle;

class QString;

class FrMainController : public FrController {
public:
    FrMainController(FrMainWindow* view = 0, FrMainDocument* doc = 0);
    ~FrMainController();
    
    void Initialize();

public:
    // Interface to tools
    bool HasActiveTool();
    FrTool* GetCurrentTool();

public:
    // Testing ... 
    void Test();

    // Interface for MainWindow
    void LoadImage(QString& fileName);
    void IoTabSettings(QString& fileName, bool isInput);
    
    void SaveCurrentViewToTab();
    void ChangeView(int view);

    void SelectLayer(int id);
	void AddLayer();
    void DeleteLayer();
    void ChangeLayer();

    void ChangeBookmark(int id);
    void DeleteBookmark(int id);

    void SetCurrentTool(int tool);

	// Override from base class
    virtual void Notify(int notifyCode);
   

    // Properties
    FrGetPropMacro(FrMainWindow*, MainView);
    FrGetPropMacro(FrMainDocument*, MainDocument);
    FrGetPropMacro(FrToolController*, ToolController);

private:
    friend class FrInteractorStyle;
};

#endif