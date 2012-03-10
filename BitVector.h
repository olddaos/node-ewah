#ifndef WVector_H
#define WVector_H

#include <v8.h>
#include <node.h>
#include "src/EWAHBoolArray/headers/ewah.h"

using namespace v8;

/// 
///  Main class, wrapping bitvector logic
///
class BitVector : public node::ObjectWrap
{
 private:

  typedef  EWAHBoolArray<>                  ewah_vector_t;
  typedef  ewah_vector_t::const_iterator    ewah_const_iterator;

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

	  //TODO: Implement map() using callback wrapping.
          //TODO: It must be drop-in replacement of built-in Array

          // Starts the iteration from scratch...
          tpl->PrototypeTemplate()->Set(String::NewSymbol("map"),
                          FunctionTemplate::New( map )->GetFunction());

	  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	  target->Set(String::NewSymbol("BitVector"), constructor);
  }

  /// Facade for underlying bitArray ( to be used when this object is unwrapped )
  void  _set( unsigned iBit )
  {
    _bitArray.set( iBit );
  }

  const ewah_vector_t&  getImmutableArray( )
  {
	return    _bitArray;
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

  /// Should work like Array.map
  static v8::Handle<v8::Value> map(const v8::Arguments& args);


  ewah_vector_t                             _bitArray;
};

#endif
