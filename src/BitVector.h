#ifndef WVector_H
#define WVector_H

#include <node.h>
#include "EWAHBoolArray/headers/ewah.h"
/// 
///  Main class, wrapping bitvector logic
///
class EWAHVector : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> target);

 private:
  EWAHVector();
  ~EWAHVector();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

 private:
 
  EWAHBoolArray<>  _bitArray;
};

#endif
