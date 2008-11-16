#ifndef FR_XML_DEFS
#define FR_XML_DEFS

// This file is needed to provide same element 
// and attribute names for both reading and saving operation
// this prevent from typos and makes writting of code easier

// Some values
#define FR_TAB_SET_VERSION 1.0
#define FR_CMTYPE_SINGLE "singleColor"
#define FR_CMTYPE_MULTI  "multiColor"
#define FR_ZEROCOLOR_VALUE "00"

// Names of elements
#define FR_XML_DECLARATION "version=\"1.0\" encoding=\"UTF-8\""
#define FR_XML_ROOT_ELEM "tabSet"
#define FR_XML_TABSETTINGS_ELEM "tabSettings"
#define FR_XML_SLICEVIEW_ELEM "sliceView"
#define FR_XML_MOSAICVIEW_ELEM "mosaicView"
#define FR_XML_ORTHOVIEW_ELEM "orthoView"
#define FR_XML_SLICENUM_ELEM "sliceNumber"
#define FR_XML_LAYERNUM_ELEM "activeLayer"
#define FR_XML_CAMS_ELEM "cameras"
#define FR_XML_CAM_ELEM "camera"
#define FR_XML_LIS_ELEM "layeredImages"
#define FR_XML_IMG_ELEM "image"
#define FR_XML_SCALE_ELEM "scale"
#define FR_XML_POS_ELEM "position"
#define FR_XML_FOCUS_ELEM "focalPoint"
#define FR_XML_VIEWUP_ELEM "viewUp"
#define FR_XML_LAYERS_ELEM "layers"
#define FR_XML_LAYER_ELEM "layer"
#define FR_XML_PXRANGE_ELEM "pxRange"
#define FR_XML_CMAP_ELEM "colorMap"
#define FR_XML_TBCSETS_ELEM "tbcSettings"
#define FR_XML_THRESH_ELEM "threshold"
#define FR_XML_BRIGHT_ELEM "brightness"
#define FR_XML_CONTRAST_ELEM "contrast"

// Names of attributes
#define FR_XML_VERSION_ATTR "version"
#define FR_XML_NAME_ATTR "name"
#define FR_XML_DESCRIPTION_ATTR "description"
#define FR_XML_ACTIVEVIEW_ATTR "activeView"
#define FR_XML_ID_ATTR "id"
#define FR_XML_VALUE_ATTR "value"
#define FR_XML_COUNT_ATTR "count"
#define FR_XML_CORONAL_ATTR "coronalValue"
#define FR_XML_SAGITAL_ATTR "sagitalValue"
#define FR_XML_AXIAL_ATTR "axialValue"
#define FR_XML_X_ATTR "x"
#define FR_XML_Y_ATTR "y"
#define FR_XML_Z_ATTR "z"
#define FR_XML_LAYERCNT_ATTR "layersCount"
#define FR_XML_OPACITY_ATTR "opacity"
#define FR_XML_VISIBLE_ATTR "visible"
#define FR_XML_MINVAL_ATTR "minValue"
#define FR_XML_MAXVAL_ATTR "maxValue"
#define FR_XML_TYPE_ATTR "type"
#define FR_XML_MIDVALUE_ATTR "midValue"
#define FR_XML_THRESH_ATTR "threshold"
#define FR_XML_COLOR_ATTR "color"

#endif