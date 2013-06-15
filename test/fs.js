describe('fs', function() {

    var chai = require('chai');

    var fs = require('../build/Release/fs');

    describe('#openSync', function() {

        it('should throw an error if file not existent', function() {
            chai.expect(function() {
                fs.openSync('blabla', fs.O_RDONLY, 0777);
            }).to.throw(Error);
        });

        it('should return a file descriptor', function() {
            chai.expect(fs.openSync('./README.md', fs.O_RDONLY, 0777))
                .to.be.a('number');
        });

    });

    describe('#readSync', function() {

        var fd;

        beforeEach(function() {
            fd = fs.openSync('./README.md', fs.O_RDONLY, 0777);
        });

        afterEach(function() {
            fs.closeSync(fd);
        });

        it('should throw an error on wrong fd', function() {
            chai.expect(function() {
                fs.readSync('', new Buffer(1), 0, 1, -1);
            }).to.throw(TypeError);
        });

        it('should throw an error on wrong buf', function() {
            chai.expect(function() {
                fs.readSync(fd, 33, 0, 1, -1);
            }).to.throw(TypeError);
        });

        it('should throw an error on wrong buf offset', function() {
            chai.expect(function() {
                fs.readSync(fd, new Buffer(1), {}, 1, -1);
            }).to.throw(TypeError);
        });
           
        it('should throw an error on wrong read length', function() {
            chai.expect(function() {
                fs.readSync(fd, new Buffer(1), 0, [], -1);
            }).to.throw(TypeError);
        });

        it('should throw an error on wrong read position', function() {
            chai.expect(function() {
                fs.readSync(fd, new Buffer(1), 0, 1, function() {});
            }).to.throw(TypeError);
        });

        it('should throw error if offset exceeds buffer length', function() {
            chai.expect(function() {
                fs.readSync(fd, new Buffer(1), 5, 1, -1);
            }).to.throw(Error);
        });

        it('should throw error if offset and length exceeds buffer', function() {
            chai.expect(function() {
                fs.readSync(fd, new Buffer(1), 1, 1, -1);
            }).to.throw(Error);
        });

        it('should read the first symbol of README.md', function() {
            var buf = new Buffer(15);

            fs.readSync(fd, buf, 0, 15, -1);

            chai.expect(buf.toString()).to.equal('# node-libuv-fs');
        });

    });

    describe('#closeSync', function() {

        it('should throw an error if first argument not a number', function() {
            chai.expect(function() {
                fs.closeSync('blabla');
            }).to.throw(TypeError);
        });

        it('should close a file', function() {
            var fd = fs.openSync('./README.md', fs.O_RDONLY, 0777);

            chai.expect(function() {
                fs.closeSync(fd);
            }).to.not.throw(Error);
        });

    });

});
