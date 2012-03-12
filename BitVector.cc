#define BUILDING_NODE_EXTENSION

#include <node.h>
#include "BitVector.h"

#include <sstream>

using namespace v8;

// Define static constructor 
Persistent<Function> BitVector::constructor;

Handle<Value> BitVector::NewInstance(const Arguments& args)
{
  HandleScope scope;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

/// Javascript new end up here, where we create BitVector instance
Handle<Value> BitVector::New( const Arguments& args )
{
  HandleScope scope;

  BitVector* obj = new BitVector();
  obj->Wrap(args.This());

  return args.This();
}

/// Appends new bit to the end of an array ( extending it if necessary )
Handle<Value> BitVector::push( const v8::Arguments& args )
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
	unsigned        iArrayCell           = 0;

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


/// Gets back total size of a bitmap in bits
v8::Handle<v8::Value> BitVector::length(const v8::Arguments& args )
{
	HandleScope scope;

	// Unwrap This to get the object to process
	BitVector* that      = ObjectWrap::Unwrap< BitVector >(args.This());
	const ewah_vector_t&   vBitArray    =  that->getImmutableArray();

	return scope.Close( Number::New( vBitArray.sizeInBits( ) ) );
}

// Joins nonzero elements with or without delimiter
v8::Handle<v8::Value> BitVector::join(const v8::Arguments& args )
{
	HandleScope   scope;

	// Default delimiter ( if none is specified )
	Handle< String >   strDelimiter  = String::New(",");

	// Check, that arguments are appropriate and consistent
	if (( args.Length() >= 1 ) && ( args[0]->IsString()) && ((args[0]->ToString())->Length( ) > 0 ))
	{
		strDelimiter = args[0]->ToString();
	}

	// That's slow, but many times faster, that Javascript Concat...
	std::stringstream  streamResult ;

	const unsigned    iLength  = strDelimiter->Utf8Length( );
	char   *pDelimiter   = new char[ iLength ];	// That was made, because original Array delimiter was not bounded by length. 
	memset( pDelimiter, 0, iLength );

	// FIXME: Delimeter can be any correct UTF8 symbol, but this method will be a bug in this case!...
	strDelimiter->WriteAscii( pDelimiter );

	// Unwrap This to get the object to process
	BitVector* that      = ObjectWrap::Unwrap< BitVector >(args.This());

	const ewah_vector_t&   vBitArray = that->getImmutableArray();	

	ewah_const_iterator       it    = vBitArray.begin( );
        streamResult              <<  *it; ++it;
        for ( ;  ( it != vBitArray.end()) && (  streamResult << pDelimiter ); ++it )
	{
		streamResult     <<  *it;
	}

	Handle< String >   strResult     = String::New( streamResult.str( ).c_str( ));

	delete  pDelimiter;
	return scope.Close( strResult );
}

v8::Handle<v8::Value> BitVector::toString(const v8::Arguments& args )
{
	HandleScope  scope;

	return   join( args );
}

v8::Handle<v8::Value> BitVector::concat(const v8::Arguments& args )
{
}

v8::Handle<v8::Value> BitVector::forEach(const v8::Arguments& args )
{
}

v8::Handle<v8::Value> BitVector::sparseand(const v8::Arguments& args )
{
}

v8::Handle<v8::Value> BitVector::rawand(const v8::Arguments& args )
{
	HandleScope scope;

        // Check, that arguments are appropriate
        if ( args.Length() < 1 )
        {
                ThrowException(Exception::TypeError(String::New("Wrong number of arguments ( must be at maximum one )")));
                return scope.Close(Undefined());
        }

        if ( !args[0]->IsObject() )
        {
                ThrowException(Exception::TypeError(String::New("Wrong type of arguments (only accepts bitVectors)")));
                return scope.Close(Undefined());
        }

	// Here we had unwrapped the second operand ( first is This )
	BitVector* rightOp   = ObjectWrap::Unwrap< BitVector >(args[0]->ToObject());

	if ( rightOp == NULL )
	{
		ThrowException(Exception::TypeError(String::New("Cannot cast arguments to something of type BitVector ( rightOp == NULL )")));
		return scope.Close(Undefined());
	}


	// Create temporable object, that we will Close after it will be filled with the result
	Handle<Value>   bvContainer       = BitVector::NewInstance( args );

	BitVector*  objContainer          = ObjectWrap::Unwrap< BitVector >(  bvContainer->ToObject() );

	// Unwrap This to get the object to process
	BitVector* that      = ObjectWrap::Unwrap< BitVector >(args.This());
	ewah_vector_t&   vThisBitArray    =  that->getMutableArray();

	//ThrowException(Exception::TypeError( strTestRight ));

	vThisBitArray.logicaland( rightOp->getMutableArray( ), objContainer->getMutableArray( ) );

	return  scope.Close( bvContainer );
}

v8::Handle<v8::Value> BitVector::rawor(const v8::Arguments& args )
{
	HandleScope scope;

        // Check, that arguments are appropriate
        if ( args.Length() < 1 )
        {
                ThrowException(Exception::TypeError(String::New("Wrong number of arguments ( must be at maximum one )")));
                return scope.Close(Undefined());
        }

        if ( !args[0]->IsObject() )
        {
                ThrowException(Exception::TypeError(String::New("Wrong type of arguments (only accepts bitVectors)")));
                return scope.Close(Undefined());
        }

	// Here we had unwrapped the second operand ( first is This )
	BitVector* rightOp   = ObjectWrap::Unwrap< BitVector >(args[0]->ToObject());

	if ( rightOp == NULL )
	{
		ThrowException(Exception::TypeError(String::New("Cannot cast arguments to something of type BitVector ( rightOp == NULL )")));
		return scope.Close(Undefined());
	}


	// Create temporable object, that we will Close after it will be filled with the result
	Handle<Value>   bvContainer       = BitVector::NewInstance( args );

	BitVector*  objContainer          = ObjectWrap::Unwrap< BitVector >(  bvContainer->ToObject() );

	// Unwrap This to get the object to process
	BitVector* that      = ObjectWrap::Unwrap< BitVector >(args.This());
	ewah_vector_t&   vThisBitArray    =  that->getMutableArray();

        // Compose dump of right and this values as strings 
	Handle< String > strBreak     = String::New(" , ");
	Handle< String > strTestRight = String::New("rightOP dump: ");

	Handle< String > strTestThis = String::New("vThisBit dump: ");

 	for ( ewah_const_iterator it    =  rightOp->getMutableArray().begin(); it != rightOp->getMutableArray().end(); ++it )
        {
		strTestRight->Concat( strTestRight, (Number::New( *it ))->ToString());
		strTestRight->Concat( strTestRight, strBreak ); 
	}	

	//ThrowException(Exception::TypeError( strTestRight ));

	vThisBitArray.logicalor( rightOp->getMutableArray( ), objContainer->getMutableArray( ) );

	return  scope.Close( bvContainer );
}

// Gets back the number of nonzero bits...
Handle<Value> BitVector::population( const v8::Arguments& args )
{
	HandleScope scope;

	// Unwrap This to get the object to process
	BitVector* that      = ObjectWrap::Unwrap< BitVector >(args.This());
	ewah_vector_t&   vBitArray    =  that->getMutableArray();

	return scope.Close( Number::New( vBitArray.numberOfOnes( ) ) );
}

// Do we really need to implement this?
Handle<Value> BitVector::get( const v8::Arguments& args )
{
   HandleScope scope;
   
   Local<Number> numFalse = Number::New( 0 );
   Local<Number> numTrue  = Number::New( 1 );

   return scope.Close( numFalse );
}


