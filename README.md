# node-ewah

Simple, fast bitmap vectors ( sparse and dense ) and indices implementation, based on Lemur project: (http://code.google.com/p/lemurbitmapindex/)

Features:

* Allows Javascript code to generate and manipulate EWAH encoded bitvectors of arbitrary length
* Word-aligned compression scheme facilitates fast encoding/decoding while retaining pretty small memory footprint
* Really speedy, thanks to the C++ background, that was wrapped to be usable in JS using some dark V8 magic :-)
* Mostly drop-in syntactic replacement for Array ( where possible and makes sense )
* Support (de)serialization of encoded bitmaps
* Well-commented and clean code

## Getting Started

Install it in node.js : 

```
npm install node-ewah

```

Then, start to play with:

```javascript
var BitVector = require('node-ewah');
```

Then create some vectors ...

```javascript

var vGoods    = new BitVector();
var vSpecies  = new BitVector();

// This is the bitvector of docids, that contain the word "goods". Bit number #xxx is on, if and only if document number #xxx contains word "goods".
vGoods.push(10);
vGoods.push(1234);
vGoods.push(101234);

// This is the bitvector of docids, that contain the word "species". Bit is the for the same reason.
vSpecies.push(1234);
vSpecies.push(1239);
vSpecies.push(10000);

// Now lets know, what documents contains first or second term
// This operation is performed using native machine words, that contain bitmap fragments, and so is extremely fast

var vMergedVector = vGoods.rawor( vSpecies );

// Check, that things are retrieved fine...
var checkVector = vGoods.map( function(x) { console.log( "From bitvector vGoods: " + x ); } );

// Check merging result...
var checkMerged = vMergedVector.map( function(x) { console.log( "From bitvector vMerged: " + x ); } );


```

## API
### push( bit_id )

Appends bit_id to the bitmap. Sorry, no random access today, but development still going hard...

### map( ... )

This thing is the same as in plain vanilla Array

### join( delimiter )

Returns catenated values of nonzero bits, interspersed with specified delimiter. Catenation is performed using C++ methods, i.e. new strings are not spawned

### toString( delimiter )

Same as above, but no delimiter is allowed :-) Pretty standard behavior...

### rawor( vectorRight ) 

Applies logical OR operation on This and vectorRight vectors. Result is new vector or undef if things were wrong.
Does not account for sparsity ( see. sparseor for that ), thus it could be slower, when applied to sparse vectors

### rawand( vectorRight ) 

Applies logical AND operation on This and vectorRight vectors. Result is new vector or undef if things were wrong


### population( ) 

Returns the number of nonzero bits ( population count ). Such a long name is to avoid confusion with pop ( which is Array-specific method ) 

### length( )

Returns compressed size ( IN BITS! ). 


## Testing

In node.js

```
> node ewah.js
```

### Benchmarking

Requires node.js


## Release notes

v.1.0 was released
