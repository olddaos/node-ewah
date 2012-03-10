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

/// Mimic Array.map ( and should provide identical behavior )
Handle<Value> BitVector::map( const v8::Arguments& args )
{
	HandleScope   scope;

	// Check, that arguments are appropriate
	if ( args.Length() < 1 )
	{
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments ( maximum one function must be specified )")));
		return scope.Close(Undefined());
	}

	if ( !args[0]->IsFunction() )
	{
		ThrowException(Exception::TypeError(String::New("Wrong type of arguments (only accepts functions)")));
		return scope.Close(Undefined());
	}

	// Unwrap This to get the object to process
	BitVector* that      = ObjectWrap::Unwrap< BitVector >(args.This());

	// Here we store things, returned from given function
        Handle< Array >      vResultArray = Array::New( );

	// Obtain a function to bother
	Local< Function >  cb = Local<Function>::Cast( args[0] );
	const unsigned  argc  = 1;

	unsigned        iArrayCell   = 0;

	const  ewah_vector_t&   vBitArray    =  that->getImmutableArray();
	for ( ewah_const_iterator it    =  vBitArray.begin(); it != vBitArray.end(); ++it )
	{
		Local<Value> argv[argc] = { Local<Value>::New( Number::New( *it )) };
		Local<Value> result = cb->Call( Context::GetCurrent()->Global(), argc, argv );
		
		vResultArray->Set( iArrayCell++, result );
	}

	// Return filled Array, as we expected to do
	return scope.Close( vResultArray );
}

Handle<Value> BitVector::get( const v8::Arguments& args )
{
   HandleScope scope;
   
   Local<Number> numFalse = Number::New( 0 );
   Local<Number> numTrue  = Number::New( 1 );

   return scope.Close( numFalse );
}


