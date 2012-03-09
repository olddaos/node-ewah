#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>

#include "BitVector.h"

// using v8::Handl
using namespace v8;
using namespace node;

/// Traditional module init function
void InitAll( v8::Handle< v8::Object> target) {
  BitVector::Init(target);
}

NODE_MODULE(node-ewah, init)
