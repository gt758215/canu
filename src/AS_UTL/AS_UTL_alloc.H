
/******************************************************************************
 *
 *  This file is part of canu, a software program that assembles whole-genome
 *  sequencing reads into contigs.
 *
 *  This software is based on:
 *    'Celera Assembler' (http://wgs-assembler.sourceforge.net)
 *    the 'kmer package' (http://kmer.sourceforge.net)
 *  both originally distributed by Applera Corporation under the GNU General
 *  Public License, version 2.
 *
 *  Canu branched from Celera Assembler at its revision 4587.
 *  Canu branched from the kmer project at its revision 1994.
 *
 *  Modifications by:
 *
 *    Brian P. Walenz from 2005-AUG-24 to 2013-AUG-01
 *      are Copyright 2005-2008,2013 J. Craig Venter Institute, and
 *      are subject to the GNU General Public License version 2
 *
 *    Brian P. Walenz from 2014-AUG-11 to 2015-JAN-31
 *      are Copyright 2014-2015 Battelle National Biodefense Institute, and
 *      are subject to the BSD 3-Clause License
 *
 *    Brian P. Walenz beginning on 2015-OCT-08
 *      are a 'United States Government Work', and
 *      are released in the public domain
 *
 *  File 'README.licenses' in the root directory of this distribution contains
 *  full conditions and disclaimers for each license.
 */

#ifndef AS_UTL_ALLOC_H
#define AS_UTL_ALLOC_H

#include "AS_global.H"


uint64    getPhysicalMemorySize(void);



const uint32  resizeArray_doNothing = 0x00;
const uint32  resizeArray_copyData  = 0x01;
const uint32  resizeArray_clearNew  = 0x02;



template<typename TT, typename LL>
void
allocateArray(TT*& array, LL arrayMax, uint32 op=resizeArray_clearNew) {

  if (array != NULL)
    delete [] array;

  array = new TT [arrayMax];

  if (op == resizeArray_clearNew)
    memset(array, 0, sizeof(TT) * arrayMax);
}



template<typename TT>
TT *
duplicateString(TT const *fr) {
  uint32  ln = strlen(fr);
  TT     *to = new TT [ln+1];

  memcpy(to, fr, sizeof(TT) * (ln+1));

  return(to);
}



template<typename TT, typename LL>
void
duplicateArray(TT*& to, LL &toLen, LL &toMax, TT *fr, LL frLen, LL UNUSED(frMax), bool forceAlloc=false) {

  if ((toMax < frLen) || (forceAlloc)) {
    delete [] to;

    toLen = frLen;
    toMax = toLen;

    to = new TT [toMax];
  }

  memcpy(to, fr, sizeof(TT) * toLen);
}


//  Set the array size to 'newMax'.  No guards, the array will ALWAYS be reallocated.

template<typename TT, typename LL>
void
setArraySize(TT*& array, uint64 arrayLen, LL &arrayMax, uint64 newMax, uint32 op=resizeArray_copyData) {

  arrayMax = newMax;

  TT *copy = new TT [arrayMax];

  if (op & resizeArray_copyData)
    memcpy(copy, array, sizeof(TT) * arrayLen);

  delete [] array;
  array = copy;

  if (op & resizeArray_clearNew)
    memset(array + arrayLen, 0, sizeof(TT) * (newMax - arrayLen));
}




//  Ensure that there is enough space to hold one more element in the array.
//  Increase the array by 'moreSpace' if needed.

template<typename TT, typename LL>
void
increaseArray(TT*& array, uint64 arrayLen, LL &arrayMax, uint64 moreSpace) {

  if (arrayLen < arrayMax)
    return;

  setArraySize(array, arrayLen, arrayMax, arrayMax + moreSpace, resizeArray_copyData);
}


//  Resize the array so that it is at least as bit as new max.  Do nothing
//  if the array is big enough already.

template<typename TT, typename LL>
void
resizeArray(TT*& array, uint64 arrayLen, LL &arrayMax, uint64 newMax, uint32 op=resizeArray_copyData) {

  if (newMax <= arrayMax)
    return;

  setArraySize(array, arrayLen, arrayMax, newMax, op);
}


template<typename T1, typename T2, typename LL>
void
resizeArrayPair(T1*& array1, T2*& array2, uint64 arrayLen, LL &arrayMax, LL newMax, uint32 op=resizeArray_copyData) {

  if (newMax <= arrayMax)
    return;

  setArraySize(array1, arrayLen, arrayMax, newMax, op);
  setArraySize(array2, arrayLen, arrayMax, newMax, op);
}


#endif // AS_UTL_ALLOC_H
