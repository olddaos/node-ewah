try {
    // node.js versions < 0.5.5
    var cc = new require('./build/default/node-ewah');
} catch(e) {
    // node.js versions >= 0.5.5
    var cc = new require('./build/Release/node-ewah');
}

var BitVector = cc.BitVector;

module.exports = BitVector;

