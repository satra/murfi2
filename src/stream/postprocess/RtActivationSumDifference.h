/******************************************************************************
 * RtActivationSumDifference.h is the header for a class that computes the
 * difference between two activation map sums
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-02-25
 *
 *****************************************************************************/

#ifndef RTACTIVATIONSUMDIFFERENCE_H
#define RTACTIVATIONSUMDIFFERENCE_H

#include"RtStreamComponent.h"
#include"RtActivation.h"
#include"RtActivationSum.h"

// class declaration
class RtActivationSumDifference : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtActivationSumDifference();

  // destructor
  ~RtActivationSumDifference();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  bool processOption(const string &name, const string &text);

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // dataID for the positive and negative activation sums
  string posActivationSumID;
  string negActivationSumID;

};

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

