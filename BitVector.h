#ifndef WVector_H
#define WVector_H

#include <v8.h>
#include <node.h>
#include "src/EWAHBoolArray/headers/ewah.h"

using namespace v8;

/// 
///  Main class, wrapping bitvector logic
///
class BitVector : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> target)
  {
	  // Prepare constructor template
	  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	  tpl->SetClassName(String::NewSymbol("BitVector"));
	  tpl->InstanceTemplate()->SetInternalFieldCount(1);

	  // Modify prototype to add bit setters/getters
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("append"),
			  FunctionTemplate::New(append)->GetFunction());

	  tpl->PrototypeTemplate()->Set(String::NewSymbol("get"),
			  FunctionTemplate::New(get)->GetFunction());

	  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	  target->Set(String::NewSymbol("BitVector"), constructor);
  }

  /// Facade for underlying bitArray
  void  _set( unsigned iBit )
  {
    _bitArray.set( iBit );
  }

  // TODO: Running using iterators
  // TODO: save&restore

 private:
  BitVector() {};
  ~BitVector() {};

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  /// Appends bit to the end of an array, automatically extending it 
  static v8::Handle<v8::Value> append(const v8::Arguments& args);

  /// Gets the status of a bit
  static v8::Handle<v8::Value> get(const v8::Arguments& args);


  EWAHBoolArray<>  _bitArray;
};

#endif
