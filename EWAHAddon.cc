#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>

#include "BitVector.h"

// using v8::Handl
using namespace v8;
using namespace node;

/// Traditional module init function
void InitAll( Handle< Object> target) {
  BitVector::Init(target);
}

extern "C" void
init(Handle<Object> target)
{
    HandleScope scope;

    BitVector::Init(target);
}


//NODE_MODULE(node-ewah, InitAll)
