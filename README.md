# node-libuv-fs

**node-libuv-fs** is a rebuilt of the **nodejs** binding to **libuv**s
filesystem api. The idea behind this is quite simple. It should just give an
idea how the simple bindings work - there is no further use case.

## Installation

You can install **node-libuv-fs** by using **npm**:

    npm install libuv-fs

If you have done this you should be able to access most common apis through the
**node-libuv-fs** module.

    var fs = require('libuv-fs');

## Documentation

In the following I will list a small api documentation about the supported 
bindings.

### fs.open(path, flags, mode)

    var fd = fs.open('./README.md', 0, 0777);
    // outputs file descriptor to console
    console.log(fd);

### fs.open(path, flags, mode, cb)

    fs.open('./README.md', 0, 0777, function(err, fd) {
        if (err) throw err;
        console.log(fd);    
    });

### fs.read(path, buffer, offset, length, position, [callback])

    fs.open('./README.md', 0, 0777, function(err, fd) {
        var buf = new Buffer(1024);
        
        fs.read(fd, buf, 0, 1024, -1, function(err, read) {
            console.log(buf);
        })
    });

### fs.close(fd, [callback])

    fs.open('./README.md', 0, 0777, function(err, fd) {
        fs.close(fd);
    });

## License

Copyright © 2013 Bodo Kaiser <bodo.rene.kaiser@googlemail.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
