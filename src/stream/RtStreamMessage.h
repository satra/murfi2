/*=========================================================================
 *  RtStreamMessage.h is the header for a class that passes messages
 *  between stream components.
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef RTSTREAMMESSAGE_H
#define RTSTREAMMESSAGE_H

//#include<dirent.h>
#include"RtData.h"
#include"RtActivation.h"
#include"RtConductor.h"

#define MAX_MSGDATAS 10000

// class declaration
class RtStreamMessage  {
 public:

  RtStreamMessage()  {
    numData = 0;
    curDataIndex = 0;
  }

  // add a peice of data to the message
  //  in
  //   pointer to the data
  //   bool for setting data as current data
  //  out
  //   success or failure
  bool addData(RtData *_data, bool _makeCurrentData = false);

  // sets a data portion by index
  //  in
  //   data pointer
  //   index of data to set
  //  out
  //   sucess or failure
  bool setData(RtData *_data, unsigned int index);

  // sets the current data to the specified index
  //  in
  //   index of data to set as current
  //  out
  //   sucess or failure
  bool setCurrentData(unsigned int index);

  // sets the last data to be current
  //  out
  //   sucess or failure
  bool setLastDataAsCurrent();

  // get a data portion by index
  //  in
  //   index of data to get
  //  out
  //   pointer to the data or NULL, if index invalid
  RtData *getData(unsigned int index) const;

  // get a data portion by a template data id. uses RtDataID::operator==
  //  in
  //   template id
  //  out
  //   pointer to the data or NULL, if such data doesnt exist
  RtData *getData(const RtDataID &idTemplate) const;

  // get a data portion by module id, data name, and roi id (returns
  // the first found instance). note that any of these can be "" to
  // match any string for that field
  //  in
  //   module id
  //   data name
  //   roi id
  //  out
  //   pointer to the data or NULL, if such data doesnt exist
  RtData *getData(const string &moduleId,
                  const string &dataName = "",
                  const string &roiId = "") const;

  // get the current data (original data plus any desired processing up to
  // this point)
  //  out
  //   pointer to the current data or NULL, if none
  RtData *getCurrentData() const;

  // get the last added data
  //  out
  //   pointer to the last data or NULL, if none
  RtData *getLastData() const;

  // get number of data objects currently stored
  unsigned int getNumData() const;

  // get pointer to our conductor
  RtConductor *getConductor() const;

  // set the pointer to our conductor
  void init(RtConductor *_conductor);

 protected:

  // pointers to data
  RtData *data[MAX_MSGDATAS];

  // how many data objs we have
  unsigned int numData;

  // index of the current data: data that represents the end result of
  // processing up this point
  unsigned int curDataIndex;

  // pointer to conductor
  RtConductor *conductor;

 private:

  // create mutex for use by addData
  ACE_Mutex mut;

};

#endif
