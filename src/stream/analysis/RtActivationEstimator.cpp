/******************************************************************************
 * RtActivationEstimator.cpp is the implementation of a base class for
 * any method for estimation of activation
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#include"RtActivationEstimator.h"

#include<gnuplot_i_vxl.h>
#include"gsl/gsl_cdf.h"

// for gamma functions
#include<vnl/vnl_gamma.h>

// mutex
#include"ace/Mutex.h"

// debugging
//#define DUMP 1
#ifdef DUMP
#include"dump.c"
#endif 

string RtActivationEstimator::moduleString("voxel-accumcor");


// default constructor
RtActivationEstimator::RtActivationEstimator() : RtStreamComponent() {

  // standard init
  componentID = moduleString;

  //trends = conditions = NULL;
  numTrends = numConditions = numMeas = 0;
  numTimepoints = 0;
  conditionShift = 0;

  modelTemporalDerivatives = false;

  modelEachBlock = false;
  blockLen = 0;

  // default values for probability thresholding
  probThreshold = 0.05;
  numComparisons = 0;
  correctForMultiComps = true;

  // default values for mask
  maskSource = THRESHOLD_FIRST_IMAGE_INTENSITY;
  mosaicMask = false;
  flipLRMask = false;
  maskIntensityThreshold = 0.5;
  putMaskOnMessage = false;

  // save t volume or not
  saveTVol = false;
  unmosaicTVol = false;
  saveTVolFilename = "t.nii";

  // mask the resulting t vol and save it as a mask or not
  savePosResultAsMask = saveNegResultAsMask = false;
  unmosaicPosMask = unmosaicPosMask = false;
  savePosAsMaskFilename = "pos_mask.nii";
  saveNegAsMaskFilename = "neg_mask.nii";

  needsInit = true;

  dumpAlgoVars = false;
  dumpAlgoVarsFilename = "log/activation_estimator_dump.txt";
}

// destructor
RtActivationEstimator::~RtActivationEstimator() {
//   if(trends != NULL) {
//     delete trends;
//   }

//   if(conditions != NULL) {
//     delete conditions;
//   }

  if(dumpAlgoVars) {
    dumpFile.close();
  }
}


// set the desired probability threshold
void RtActivationEstimator::setProbThreshold(double p) {
  probThreshold = p;
}

// get the desired probability threshold
double RtActivationEstimator::getProbThreshold() {
  return probThreshold;
}

// set whether stat thresholds should reflect multiple comparisons corrections
void RtActivationEstimator:: setCorrectMultipleComparisons(bool correct) {
  correctForMultiComps = correct;
}

// get whether stat thresholds should reflect multiple comparisons corrections
bool RtActivationEstimator::getCorrectMultipleComparisons() {
  return correctForMultiComps;
}

// get the desired t statistic threshold
double RtActivationEstimator::getTStatThreshold(unsigned int dof) {
  return fabs(gsl_cdf_tdist_Pinv(probThreshold 
		    / (correctForMultiComps ? numComparisons : 1.0),dof));
}

// process a configuration option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtActivationEstimator::processOption(const string &name, const string &text) {

  // look for known options
  if(name == "condition") { // load the condition vector
    numConditions++;

    if(numConditions == 1) { // allocate condition matrix
      conditions.clear();
      conditions.set_size(numMeas,MAX_CONDITIONS);
    }
    else if(numConditions > MAX_CONDITIONS) {
      cerr << "warning: max number of conditions exceeded." << endl;
      return false;
    }
    
    double el;
    size_t i = 0;
    for(size_t i1 = 0, i2 = text.find(" "); 1; 
	i++, i1 = i2+1, i2 = text.find(" ", i1)) {

      if(!RtConfigVal::convert<double>(el, 
	  text.substr(i1,i2 == string::npos ? text.size()-i1 : i2-i1))) {
	continue;
      }
      conditions.put(i,numConditions-1,el);

      if(i2 == string::npos) { // test if we are on the last one
	// set the blockLen if its not been set
	if(blockLen == 0) {
	  blockLen = i+1;
	  cout << "auto setting blockLen to " << blockLen << endl;
	}

	break;
      }
    }

    // fill the rest of the measurements as periodic stim
    for(; i < numMeas; i++) {
      conditions.put(i,numConditions-1,conditions.get(i%blockLen,0));
    }



    return true;
  }
  if(name == "conditionshift") {
    return RtConfigVal::convert<unsigned int>(conditionShift,text);
  }
  if(name == "modelTemporalDerivatives") {
    return RtConfigVal::convert<bool>(modelTemporalDerivatives,text);
  }
  if(name == "modelEachBlock") {
    return RtConfigVal::convert<bool>(modelEachBlock,text);
  }
  if(name == "trends") {
    return RtConfigVal::convert<unsigned int>(numTrends,text);
  }
  if(name == "probThreshold") {
    return RtConfigVal::convert<double>(probThreshold,text);
  }  
  if(name == "correctForMultiComps") {
    return RtConfigVal::convert<bool>(correctForMultiComps,text);
  }  
  if(name == "maskSource") {
    // match type string
    if(text == "thresholdFirstImageIntensity") {
      maskSource = THRESHOLD_FIRST_IMAGE_INTENSITY;
    }
    else if(text == "loadFromFile") {
      maskSource = LOAD_FROM_FILE;
    }
    else if(text == "passedFromInside") {
      maskSource = PASSED_FROM_INSIDE;
    }
  }
  if(name == "maskFilename") {
    mask.setFilename(text);
    return true;
  }
  if(name == "mosaicMask") {
    RtConfigVal::convert<bool>(mosaicMask,text);
  }
  if(name == "flipLRMask") {
    RtConfigVal::convert<bool>(flipLRMask,text);
  }
  if(name == "maskToDisplay") {
    RtConfigVal::convert<bool>(putMaskOnMessage,text);
  }
  if(name == "roiID") {
    roiID = text;
    return true;
  }  
  if(name == "maskIntensityThreshold") {
    return RtConfigVal::convert<double>(maskIntensityThreshold,text);
  }
  if(name == "saveTVol") {
    return RtConfigVal::convert<bool>(saveTVol,text);
  }
  if(name == "unmosaicTVol") {
    return RtConfigVal::convert<bool>(unmosaicTVol,text);
  }
  if(name == "saveTVolFilename") {
    saveTVolFilename = text;
    return true;
  }
  if(name == "savePosAsMask") {
    return RtConfigVal::convert<bool>(savePosResultAsMask,text);
  }
  if(name == "unmosaicPosMask") {
    return RtConfigVal::convert<bool>(unmosaicPosMask,text);
  }
  if(name == "saveNegAsMask") {
    return RtConfigVal::convert<bool>(saveNegResultAsMask,text);
  }
  if(name == "unmosaicNegMask") {
    return RtConfigVal::convert<bool>(unmosaicNegMask,text);
  }
  if(name == "savePosAsMaskFilename") {
    savePosAsMaskFilename = text;
    return true;
  }
  if(name == "saveNegAsMaskFilename") {
    saveNegAsMaskFilename = text;
    return true;
  }
  if(name == "dumpAlgoVars") {
    return RtConfigVal::convert<bool>(dumpAlgoVars,text);
  }
  if(name == "dumpAlgoVarsFilename") {
    dumpAlgoVarsFilename = text;
    return true;
  }

  return RtStreamComponent::processOption(name, text);
}  


// process gloabl config info or config from other modules
bool RtActivationEstimator::processConfig(RtConfig &config) {
  
  if(config.isSet("scanner:measurements")) {
    numMeas = config.get("scanner:measurements");
  } 
  if(config.isSet("scanner:tr")) {
    tr= config.get("scanner:tr");
  } 
  else {
    cerr << "error: number of measurements has not been set" << endl;
    return false;
  }

  return true;
}

// build a gamma pdf with shape and scale parms
// see http://www.itl.nist.gov/div898/handbook/apr/section1/apr165.htm
double gammaPDF(double t, double a, double b) {
  return pow(b,a)/vnl_gamma(a) * pow(t,a-1) * exp(-b*t);
}


// builds an hrf vector from the difference of two gammas
//
// in
//  tr:           repetition time in seconds
//  samplePeriod: temporal precision in seconds
//  length:       length of the HRF in seconds
// out
//  vnl_vector HRF
void RtActivationEstimator::buildHRF(vnl_vector<double> &hrf,
				     double tr,
				     double samplePeriod, 
				     double length) {

  double timeToPeakPos = 6;
  double timeToPeakNeg = 16;
  double posToNegRatio = 6;

  double dt = tr*samplePeriod;
  hrf.set_size((int)ceil(length/tr+1));
  for(unsigned int i = 0, t = 0; t <= (unsigned int) ceil(length/samplePeriod); 
      t += (unsigned int) ceil(tr/samplePeriod), i++) {
    hrf.put(i, gammaPDF(t/tr,timeToPeakPos,dt)
  	    - gammaPDF(t/tr,timeToPeakNeg,dt)/posToNegRatio);
      
  }
  hrf.normalize();

//  // sum
//  for(unsigned int i = 0; i < hrf.size(); i++) {
//    cout << hrf[i] << endl;
//  }
}

// finish initialization and prepare to run
bool RtActivationEstimator::finishInit() {
  buildTrends();
  buildConditions();

  // mask from file
  if(maskSource == LOAD_FROM_FILE) {
    if(mask.load()) {
      cout << "loaded mask from " << mask.getFilename() << endl;
    }
    else {
      cout << "failed to load mask from " << mask.getFilename() << endl;
      putMaskOnMessage = false;
    }

    // mosaic if we need to
    if(mosaicMask 
       || ((int) mask.getNumPix() > mask.getDim(0) * mask.getDim(1))) {
      //cout << "warning: auto mosaic" << endl;
      mask.mosaic();
    }

    // flip if we need to
    if(flipLRMask) {
      mask.flipLR();
    }
  }

  // open the dump file
  if(dumpAlgoVars) {
    dumpFile.open(dumpAlgoVarsFilename.c_str());
    if(dumpFile.fail()) {
      cout << "couldn't open file to dump vars" << endl;
      dumpAlgoVars = false;
    }
    else {
      cout << "opened file " << dumpAlgoVarsFilename << " to dump vars" << endl;
      startDumpAlgoVarsFile();
    }
  }

  return true;
}

#include"printVnlMat.cpp"
#include"printVnlVector.cpp"

// divide by range
#define unitDeviation(v) v/=(v.max_value()-v.min_value())

// build the condition regressors
void RtActivationEstimator::buildConditions() {
  // convolve the conditions with hrf (cannonical from SPM)
  vnl_vector<double> hrf;
  buildHRF(hrf, tr, 1/16.0, 32);
  //  printVnlVector(hrf);

  // debugging
//static Gnuplot g1;
//g1 = Gnuplot("lines");
//g1.plot_x(hrf,"hrf");

  // incorporate condition shift
  if(conditionShift > 0 && conditionShift < numMeas) {
    vnl_vector<double> shiftdelta(numMeas,0.0);
    shiftdelta.put(conditionShift,1);

    for(unsigned int i = 0; i < numConditions; i++) {
      vnl_vector<double> col = conditions.get_column(i);
      vnl_vector<double> shiftcol = vnl_convolve(col,shiftdelta);
      col.update(shiftcol.extract(col.size()));

      conditions.set_column(i,col);
    }  
  }

  // build block-wise condition vectors if required
  if(modelEachBlock) {
    int numBlocks = (numMeas-conditionShift)/blockLen;
    
    // make a new condition matrix
    vnl_matrix<double> newConds(numMeas, numConditions*numBlocks);
    newConds.fill(0);
    for(unsigned int meas=conditionShift; meas < numMeas; meas++) {
      unsigned int block = (meas-conditionShift)/blockLen;
      for(unsigned int cond=0; cond < numConditions; cond++) {
	newConds.put(meas, cond*numBlocks+block, conditions[meas][cond]);
      }
    }
//     cout << "conds" << endl;
//     printVnlMat(conditions);
//    cout << endl;

    conditions = newConds;
    numConditions*=numBlocks;

//     cout << "new conds" << endl;
//     printVnlMat(conditions);
//     cout << endl;
  }

  // convolve each condition with the hrf
  for(unsigned int i = 0; i < numConditions; i++) {
    vnl_vector<double> col = conditions.get_column(i);
    vnl_vector<double> convhrfcol = vnl_convolve(col,hrf);
    col.update(convhrfcol.extract(col.size()));

    // make total deviation one
    unitDeviation(col);

    conditions.set_column(i,col);
  }

  // add columns for temporal derivatives if required
  if(modelTemporalDerivatives) {
    // make a new condition matrix
    vnl_matrix<double> newConds(numMeas, 2*numConditions);
    newConds.fill(0);

    for(unsigned int cond=0; cond < numConditions; cond++) {
      // copy regressor of interest
      newConds.set_column(2*cond, conditions.get_column(cond));

      // compute derivative for one condition for each measurement
      for(unsigned int meas=1; meas < numMeas; meas++) {
	newConds.put(meas, 2*cond+1, 
		     conditions[meas][cond]-conditions[meas-1][cond]);
      }
    }
//     cout << "conds" << endl;
//     printVnlMat(conditions);
//    cout << endl;

    conditions = newConds;
    numConditions*=2;    

  }
  //printVnlMat(conditions);

}

// test if a condition index is a derivative (index ignores trend regressors)
bool RtActivationEstimator::conditionIsDerivative(unsigned int index) {
  return modelTemporalDerivatives && index % 2;
}

// build the trend regressors
void RtActivationEstimator::buildTrends() {
  trends.clear();
  trends.set_size(numMeas, numTrends);

  for(unsigned int i = 0; i < numTrends; i++) {
    for(unsigned int j = 0; j < numMeas; j++) {
      switch(i) {
      case 0: // mean
	trends.put(j,i,1.0);
	break;
      case 1: // linear
	trends.put(j,i,j+1);
	break;
      default:
	trends.put(j,i,0.0);
	break;
      }
    }
  }
}

// initialize the estimation algorithm for a particular image size
// in
//  first acquired image to use as a template for parameter inits
void RtActivationEstimator::initEstimation(RtMRIImage &image) {
  // mask from intensity threshold
  if(maskSource == THRESHOLD_FIRST_IMAGE_INTENSITY) {
    numComparisons 
      = mask.initByMeanIntensityThreshold(image, maskIntensityThreshold);
  }

  needsInit = false;
}

// sets the latest result of processing
//  in
//   data result
void RtActivationEstimator::setResult(RtStreamMessage *msg, RtData *data) {
  // send the mask if desired 
  if(putMaskOnMessage) {
    mask.setRoiID(roiID);
    //cout << "sending mask: " << mask.getID() << ":" << roiID << endl;

    ACE_Mutex mut;
    mut.acquire();
    passData(&mask);
    mut.release();
  }

  RtStreamComponent::setResult(msg,data);
}
  
// start a logfile 
void RtActivationEstimator::startDumpAlgoVarsFile() {
  dumpFile << "started at ";
  printNow(dumpFile);
  dumpFile << endl
	   << "print variable names here separated by spaces "
	   << "end" << endl;  
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


