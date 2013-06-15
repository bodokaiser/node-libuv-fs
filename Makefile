test: test-addon

build: build-addon

install:
	npm install

test-addon:
	./node_modules/.bin/mocha \
	 	test/*.js

build-addon:
	./node_modules/.bin/node-gyp \
		configure build
