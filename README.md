# node-ewah

Simple, fast bitmap vectors ( sparse and dense ) and indices implementation, based on Lemur project: (http://code.google.com/p/lemurbitmapindex/)

Features:

* Allows Javascript code to generate and manipulate EWAH encoded bitvectors of arbitrary length, thus performing fast and space efficient computations
* Really speedy, thanks to the C++ background, that was wrapped to be usable in JS using some dark V8 magic :-)
* Support (de)serialization of encoded bitmaps
* Should have been working in all browsers as well as under node.js, however, this should be tested with more care
* [Annotated source code](http://broofa.github.com/node-uuid/docs/uuid.html)

## Getting Started

Install it in node.js:

```
npm install node-ewah
```

```javascript
var Bitvector = require('node-ewah');
```

Then create some vectors ...

```javascript

var vGoods    = new Bitvector();
var vSpecies  = new Bitvector();

// This is the bitvector of docids, that contain the word "goods". Bit number #xxx is on, if and only if document number #xxx contains word "goods".
vGoods.set(10);
vGoods.set(1234);
vGoods.set(101234);

// This is the bitvector of docids, that contain the word "species". Bit is the for the same reason.
vSpecies.set(1234);
vSpecies.set(1239);
vSpecies.set(10000);

// Now lets know, what documents contains both terms at the same time
// This operation is performed using native machine words, that contain bitmap fragments, and so is extremely fast

vMergedVector = vGoods.and( vSpecies );

```

## API


## Testing

In node.js

```
> cd test
> node ewah.js
```

### Benchmarking

Requires node.js


## Release notes

v.1.0 was released
