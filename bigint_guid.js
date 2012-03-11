
var generator = require('node-uuid');                                                                                                                                                                               
var bigint    = require('bigint');

// Dirty hack for a JS to stop thinking, that it have a signed integer
function dropneg(x)
{
   return ( x >>> 0 );
}

/// Hacky left shift, clearing internal "signed" flag, and allowing performing C++ styled shifts
/// The main point of this function is that it *indeed* performs shifting, and NOT multiplications. This saves about 10000 ( or more ) ticks
function ushift( x, r )
{
   return (( x << r) >>> 0);
}

/// Yeah, this is just more beautiful ( yet less effecient ) version of ushift. 
function plshift(x, r) {
    var P = ((x << r) < 0),
        R = ((x << r) >>> P) * (1 + P);

    return R;
}

var NAMESPACE_MAPPING = {
    'data.rian.ru' : 1,
    'visualrian' : 2
}

var guid = module.exports = function(version, owner, uuid, reserved) {

    if(arguments.length === 1) {
        this.parse(this.guid = arguments[0]);
    } else {
        this.version = version || 1;
        this.namespace = NAMESPACE_MAPPING[owner] || 0;
        this.uuid = uuid || 0;
        this.reserved = reserved || 0;
    }

};

var ptp = guid.prototype;

ptp.generate = function() {
    var buf = new Buffer(6);

    this.uuid = bigint( 0 );

    generator.v4(null, buf, 0);

    /*
        Обрабатываем первые 5 байт
        @see http://rm.info.rian.ru/projects/metadata/wiki/#guid-человекочитаемые-URI-новостей-и-изоморфизм-между-ними
    */
    for (var i = 0, j = 5; i < j; i++) {
        this.uuid = this.uuid.or( buf[i]);
        // this.uuid <<= 8;
        this.uuid = this.uuid.shiftLeft( 8 );
    }

    // Обрабатываем последние 5 бит из 6-го байта uuid-а (45 бит)
    this.uuid = this.uuid.or( buf[5] & 0xF8 );

    return (this.guid = this.build());
};

/// Pretty printing of hex values...
ptp.hexlog = function(x) {
   console.log( x.toString(16).toUpperCase() );
};

ptp.build = function() {
    // TODO: this function have a bug, that appears on 32-bit systems. Unlike C++ ( which have a type like 'long' ), JS can only shift, using 32 bits
    var version     = bigint( this.version ).shiftLeft( 55 );
    var littleUUID  = bigint( this.uuid ).shiftLeft( 10 );
    var ns          = bigint( this.namespace ).shiftLeft( 2 );

    var guidJoined  = version.or( littleUUID );
    guidJoined      = guidJoined.or( ns );
    guidJoined      = guidJoined.or( this.reserved );

    return      guidJoined.toNumber( );  
}

ptp.parse = function(guid) 
{
  var initialGuid = bigint(guid);

  var version     = initialGuid.shiftRight( 55 );
  var uuid        = (initialGuid.shiftLeft( 9 )).shiftRight( 19 );    // Kill version, retain uuid
  var ns          = ( initialGuid.shiftLeft( 54 )).shiftRight( 56 );

  return {
	"version"   : version,
	"uuid"      : uuid,
        "namespace" : ns
  };
};

ptp.increment = function() {
    return this.version++ && this.build();
};

