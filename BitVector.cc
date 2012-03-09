#define BUILDING_NODE_EXTENSION

#include <node.h>
#include "BitVector.h"

using namespace v8;

/// Javascript new end up here, where we create BitVector instance
Handle<Value> BitVector::New( const Arguments& args )
{
  HandleScope scope;

  BitVector* obj = new BitVector();
  obj->Wrap(args.This());

  return args.This();
}

/// Appends new bit to the end of an array ( extending it if necessary )
Handle<Value> BitVector::append( const v8::Arguments& args )
{
	HandleScope scope;

	BitVector* obj = ObjectWrap::Unwrap< BitVector >(args.This());

	if (args.Length() < 1)
	{
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}

	if ( !args[0]->IsNumber() )
	{
                ThrowException(Exception::TypeError(String::New("Wrong type of arguments")));
                return scope.Close(Undefined());
	}

	const unsigned iBit = args[0]->NumberValue( );
	obj->_set( iBit );

	// We have nothing to return, as obj->set is void!
        return scope.Close( Undefined());
}

Handle<Value> BitVector::get( const v8::Arguments& args )
{
   HandleScope scope;
   
   Local<Number> numFalse = Number::New( 0 );
   Local<Number> numTrue  = Number::New( 1 );

   return scope.Close( numFalse );
}


