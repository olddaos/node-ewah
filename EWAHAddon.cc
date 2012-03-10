#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>

#include "BitVector.h"

// using v8::Handl
using namespace v8;
using namespace node;

/// Factory method to allow creating BitVectors in its methods
Handle<Value> CreateObject( const Arguments& args ) 
{
  HandleScope scope;
  return scope.Close( BitVector::NewInstance(args) );
}

extern "C" void
init(Handle<Object> target)
{
    HandleScope scope;

    BitVector::Init(target);

    // Install object creator
    target->Set(String::NewSymbol("createObject"),
   	   FunctionTemplate::New(CreateObject)->GetFunction());
}


//NODE_MODULE(node-ewah, InitAll)
