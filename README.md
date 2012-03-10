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

Install it in node.js ( npm support is upcoming ):

```
Clone repository:
git clone git@github.com:olddaos/node-ewah.git

Assemble using WAF:
node-waf configure build

All done...
```

```javascript
var BitVector = require('node-ewah');
```

Then create some vectors ...

```javascript

var vGoods    = new BitVector();
var vSpecies  = new BitVector();

// This is the bitvector of docids, that contain the word "goods". Bit number #xxx is on, if and only if document number #xxx contains word "goods".
vGoods.append(10);
vGoods.append(1234);
vGoods.append(101234);

// This is the bitvector of docids, that contain the word "species". Bit is the for the same reason.
vSpecies.append(1234);
vSpecies.append(1239);
vSpecies.append(10000);

// Now lets know, what documents contains both terms at the same time
// This operation is performed using native machine words, that contain bitmap fragments, and so is extremely fast
// In addition, sparse_and c
vMergedVector = vGoods.sparse_and( vSpecies );

// Check, that things are retrieved fine...
var checkVector = vGoods.map( function(x) { console.log( "From bitvector vGoods: " + x ); } );

// Check merging result...
var checkMerged = vMergedVector.map( function(x) { console.log( "From bitvector vMerged: " + x ); } );


```

## API
### append( bit_id )

Appends bit_id to the bitmap. Sorry, no random access today, but is developed hard...

### map( ... )

This thing is the same as in plain vanilla Array

## Testing

In node.js

```
> node ewah.js
```

### Benchmarking

Requires node.js


## Release notes

v.1.0 was released
