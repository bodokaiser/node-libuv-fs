describe('fs', function() {

    var chai = require('chai');

    var fs = require('../build/Release/fs');

    describe('#open', function() {

        it('should throw an error if file not existent', function() {
            chai.expect(function() {
                fs.open('blabla', 0, 0777);
            }).to.throw(Error);

            /*
            fs.open('balbal', 0, 0777, function(err) {
                chai.expect(err).to.be.an.instanceof(Error);
            });
            */
        });

        it('should return a file descriptor', function() {
            chai.expect(fs.open('./README.md', 0, 0777))
                .to.be.a('number');

            /*
            fs.open('./README.md', 0, 0777, function(err, fd) {
                chai.expect(fd).to.be.a('number');
                fs.close(fd);
            });
            */
        });

    });

    describe('#read', function() {

        var fd;

        beforeEach(function() {
            fd = fs.open('./README.md', 0, 0777);
        });

        afterEach(function() {
            fs.close(fd);
        });

        it('should throw an error on wrong fd', function() {
            chai.expect(function() {
                fs.read('', new Buffer(1), 0, 1, -1);
            }).to.throw(TypeError);
        });

        it('should throw an error on wrong buf', function() {
            chai.expect(function() {
                fs.read(fd, 33, 0, 1, -1);
            }).to.throw(TypeError);
        });

        it('should throw an error on wrong buf offset', function() {
            chai.expect(function() {
                fs.read(fd, new Buffer(1), {}, 1, -1);
            }).to.throw(TypeError);
        });
           
        it('should throw an error on wrong read length', function() {
            chai.expect(function() {
                fs.read(fd, new Buffer(1), 0, [], -1);
            }).to.throw(TypeError);
        });

        it('should throw an error on wrong read position', function() {
            chai.expect(function() {
                fs.read(fd, new Buffer(1), 0, 1, function() {});
            }).to.throw(TypeError);
        });

        it('should throw error if offset exceeds buffer length', function() {
            chai.expect(function() {
                fs.read(fd, new Buffer(1), 5, 1, -1);
            }).to.throw(Error);
        });

        it('should throw error if offset and length exceeds buffer', function() {
            chai.expect(function() {
                fs.read(fd, new Buffer(1), 1, 1, -1);
            }).to.throw(Error);
        });

        it('should read the first symbol of README.md', function() {
            var buf = new Buffer(15);
            /*
            fs.read(fd, buf, 0, 15, -1, function(err, read) {
                chai.expect(buf.toString())
                    .to.equal('# node-libuv-fs');
            });
            */
        });

    });

    describe('#close', function() {

        it('should throw an error if first argument not a number', function() {
            chai.expect(function() {
                fs.close('blabla');
            }).to.throw(TypeError);
        });

        it('should close a file', function() {
            var fd = fs.open('./README.md', 0, 0777);

            chai.expect(function() {
                fs.close(fd);
            }).to.not.throw(Error);
        });

    });

});
