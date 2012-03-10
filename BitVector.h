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

  // Factory method to create instances without JavaScript new
  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);

  static void Init(v8::Handle<v8::Object> target)
  {
	  // Prepare constructor template
	  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	  tpl->SetClassName(String::NewSymbol("BitVector"));
	  tpl->InstanceTemplate()->SetInternalFieldCount(1);

	  // NB: Following methods are implemented only to provide syntactic similarity and to avoid confusion with brand new API
	  // Implements push in the same sense, as in usual Arraya ( but no pop, of course ) 
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("push"),
			  FunctionTemplate::New( push )->GetFunction());


	  // DONE: This methods just returns the complete number of bits, which is necessary to store the datastructure
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("length"),
			  FunctionTemplate::New( length )->GetFunction());

	  // TODO: Performs join exactly in the same manner, as Array do
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("join"),
			  FunctionTemplate::New( join )->GetFunction());


	  // TODO: Just stringifies numbers of a set bits
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
			  FunctionTemplate::New( toString )->GetFunction());


	  // TODO: concatenate bitmaps. The same semantics, as Array.concat.
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("concat"),
			  FunctionTemplate::New( concat )->GetFunction());

	  // TODO: perform forEach exactly as Array would do
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("forEach"),
			  FunctionTemplate::New( forEach )->GetFunction());


          // DONE: Starts the iteration from scratch...
          tpl->PrototypeTemplate()->Set(String::NewSymbol("map"),
                          FunctionTemplate::New( map )->GetFunction());

	  // NB: Following are methods, that are specific for bitVector 
	  // and makes the sole sense of using it! :-)

	  // TODO: Logical AND ( accounts for sparsity, and promised to be faster ) between This object and object from Args
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("sparseand"),
			  FunctionTemplate::New( sparseand )->GetFunction());

	  // DONE: Logical AND ( regardless sparsity) between This object and given as an argument
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("rawand"),
			  FunctionTemplate::New( rawand )->GetFunction());

	  // DONE: Logical OR ( regardless sparsity )
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("rawor"),
			  FunctionTemplate::New( rawor )->GetFunction());

	  // DONE: Population counter
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("population"),
			  FunctionTemplate::New( population )->GetFunction());

	  // Now this this is static
  	  constructor = Persistent<Function>::New(tpl->GetFunction());

//	  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
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

  // Sorry for that, guys... One needs to copy it first, but not to expose naked and unprotected...
  ewah_vector_t&  getMutableArray( )
  {
	return    _bitArray;
  }

  // TODO: Running using iterators
  // TODO: save&restore

 private:
  BitVector() {};
  ~BitVector() {};

  // This will construct our objects when we need to return them
  static v8::Persistent<v8::Function> constructor;

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  /// DONE: Appends bit to the end of an array, automatically extending it 
  static v8::Handle<v8::Value> push(const v8::Arguments& args);

  /// Gets the status of a bit
  static v8::Handle<v8::Value> get(const v8::Arguments& args);

  static v8::Handle<v8::Value> length(const v8::Arguments& args);

  /// DONE: Should work like Array.map
  static v8::Handle<v8::Value> map(const v8::Arguments& args);

  static v8::Handle<v8::Value> join(const v8::Arguments& args);

  static v8::Handle<v8::Value> toString(const v8::Arguments& args);

  static v8::Handle<v8::Value> concat(const v8::Arguments& args);

  static v8::Handle<v8::Value> forEach(const v8::Arguments& args);

  static v8::Handle<v8::Value> sparseand(const v8::Arguments& args);

  static v8::Handle<v8::Value> rawand(const v8::Arguments& args);

  static v8::Handle<v8::Value> rawor(const v8::Arguments& args);

  // DONE: Gets back population number
  static v8::Handle<v8::Value> population(const v8::Arguments& args);
  ewah_vector_t                             _bitArray;
};

#endif
