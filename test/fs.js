describe('fs', function() {

    var chai = require('chai');

    var fs = require('../build/Release/fs');

    describe('#open', function() {

        it('should throw an error if file not existent', function() {
            chai.expect(function() {
                fs.open('blabla');
            }).to.throw(Error);
        });

        it('should return a file descriptor', function() {
            chai.expect(fs.open('./README.md')).to.be.a('number');
        });

    });

    describe('#read', function() {

    });

    describe('#close', function() {

        it('should throw an error if first argument not a number', function() {
            chai.expect(function() {
                fs.close('blabla');
            }).to.throw(TypeError);
        });

        it('should close a file', function() {
            var fd = fs.open('./README.md');

            chai.expect(function() {
                fs.close(fd);
            }).to.not.throw(Error);
        });

    });

});
