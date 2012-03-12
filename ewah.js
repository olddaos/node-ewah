#! /usr/bin/env node

var addon = require('./build/Release/node-ewah');

var bv1    = new addon.BitVector();

var bv2    = addon.createObject();

bv1.push( 10 );
bv1.push( 678 );
bv1.push( 2678 );

bv2.push( 10 );
bv2.push( 512 );
bv2.push( 4096 );

//var bv3   = addon.createObject( );

// Result should came in bv3
var bvOR   = bv1.rawor( bv2 );

var bvAND  = bv1.rawand( bv2 );

//console.log( "bv3 = " + bv3 );
console.log("\n ------ joined bv1: \n");
console.log( bv1.join());

var checkVector  = bv1.map( function(x) { console.log( "From bv1: " + x ); } );

console.log("\n ------------------ \n");
var checkVector2 = bv2.map( function(x) { console.log( "From bv2: " + x ); } );

console.log("\n ------------------ \n");
var testOrVector = bvOR.map(  function(x) { console.log( "From bvOR: " + x ); } );

console.log("\n ------ joined bvOR: \n");
console.log( bvOR.join(" ; "));

console.log("\n ------ stringified bvOR: \n");
console.log( bvOR.toString());

console.log("\n ------------------ \n");
var testANDVector = bvAND.map(  function(x) { console.log( "From bvAND: " + x ); } );

console.log("\n ------------------ \n");
console.log( "bv1 populaion = " + bv1.population( ) + " bv1.size = " + bv1.length() );

console.log( "bv2 populaion = " + bv2.population( ) + " bv2.length = " + bv2.length() );

console.log( "bvOR populaion = " + bvOR.population( ) + " bvOR length = " + bvOR.length() );
