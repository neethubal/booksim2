// $Id$

/*
 Copyright (c) 2007-2015, Trustees of The Leland Stanford Junior University
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this 
 list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, this
 list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// ----------------------------------------------------------------------
//
//  Queuing: Actually Queuing Arbiter
//
// ----------------------------------------------------------------------

#include "actually_queuing_arb.hpp"
#include <iostream>
#include <limits>
#include "random_utils.hpp"
using namespace std ;

ActuallyQueingArbiter::ActuallyQueingArbiter( Module *parent, const string &name,
				      int size ) 
  : Arbiter( parent, name, size ) {
    _pick_limits.resize(size);
}


void ActuallyQueingArbiter::AddRequest( int input, int id, int pri, int ctime)
{
  int prio = (ctime < 0) ? 0 : (numeric_limits<int>::max() - ctime);
  Arbiter::AddRequest(input, id, prio, ctime);
}

void ActuallyQueingArbiter::PrintState() const  {
  cout << "Queuing arbiter has no real state. There is no queue, age comp is done at arbitration" << endl ;
}

void ActuallyQueingArbiter::UpdateState() {
  if ( _selected > -1 )
    _request[_selected].valid = 0;
}

int ActuallyQueingArbiter::Arbitrate( int* id, int* pri ) {
  
  int max_prio = -1;
  for ( int i = 0; i < _size ; i++ ) {
    if (_request[i].valid && (_request[i].pri > max_prio || (_request[i].pri == max_prio && RandomInt(1)))) {
      max_prio = _request[i].pri;
      _selected = i;
    }
  }
  return Arbiter::Arbitrate(id, pri);
}

void ActuallyQueingArbiter::Clear()
{
  Arbiter::Clear();
}
