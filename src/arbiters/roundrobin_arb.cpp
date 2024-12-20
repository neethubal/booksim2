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
//  RoundRobin: RoundRobin Arbiter
//
// ----------------------------------------------------------------------

#include "roundrobin_arb.hpp"
#include <iostream>
#include <limits>

using namespace std ;

RoundRobinArbiter::RoundRobinArbiter( Module *parent, const string &name,
				      int size ) 
  : Arbiter( parent, name, size ), _pointer( 0 ) {
}

void RoundRobinArbiter::PrintState() const  {
  cout << "Round Robin Priority Pointer: " << endl ;
  cout << "  _pointer = " << _pointer << endl ;
}

void RoundRobinArbiter::UpdateState() {
  // update priority matrix using last grant
  if ( _selected > -1 ) 
    _pointer = ( _selected + 1 ) % _size ;
}

void RoundRobinArbiter::AddRequest( int input, int id, int pri, int ctime)
{
  if(!_request[input].valid || (_request[input].pri < pri)) {
    if((_num_reqs == 0) || 
       Supersedes(input, pri, _best_input, _highest_pri, _pointer,_size )) {
      _highest_pri = pri;
      _best_input = input;
    }
  }
  Arbiter::AddRequest(input, id, pri, -1);
}

int RoundRobinArbiter::Arbitrate( int* id, int* pri ) {
  
  /* 
  int any_valid = 0;
  int diff_prio = 0;
  int last_prio = -1;
  for ( int i = 0 ; i < _size; i++ ) {
    any_valid += _request[i].valid;
    if (_request[i].valid) {
      diff_prio += (last_prio != _request[i].pri) && (last_prio >= 0);
      last_prio = _request[i].pri;
    }
  }

  if (any_valid >= 2 && diff_prio > 0) {
  cout << "Request state: " << _best_input << " | ";
  for ( int i = 0 ; i < _size ; i++ )
    if (_request[i].valid)
     cout << i << " " << _request[i].id << " " << numeric_limits<int>::max() - _request[i].pri << " | " ;
    cout << "\n";
  }
  */
  _selected = _best_input;
  
  return Arbiter::Arbitrate(id, pri);
}

void RoundRobinArbiter::Clear()
{
  _highest_pri = numeric_limits<int>::min();
  _best_input = -1;
  Arbiter::Clear();
}
