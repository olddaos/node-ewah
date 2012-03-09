#! /usr/bin/env node

var addon = require('./build/Release/node-ewah');

var bv    = new addon.BitVector();

bv.append( 10 );
bv.append( 678 );
bv.append( 2678 );

