test: test-addon

build: build-addon

MOCHA_FLAGS = --require chai

install:
	npm install

test-addon:
	./node_modules/.bin/mocha \
		$(MOCHA_FLAGS) test/*.js

build-addon:
	./node_modules/.bin/node-gyp \
		configure build
