CC=g++
CFLAGS=-Wall -std=c++17 -O3 -funroll-loops -DASIO_STANDALONE

INCLUDES=-Iinclude

SRC=src/server.cpp src/board.cpp src/butils.cpp src/bdata.cpp src/engine.cpp src/uciws.cpp src/rollerball.cpp

rollerball:
	mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -lpthread -o bin/rollerball

package:
	mkdir -p build 
	mkdir -p build/rollerball
	rm -f build/rollerball.zip
	rm -rf build/rollerball/*
	mkdir build/rollerball/src build/rollerball/websrc
	cp -r include build/rollerball/include
	cp src/*.hpp build/rollerball/src/
	cp $(SRC) build/rollerball/src/
	cp -r scripts build/rollerball/scripts
	cp Makefile README.md build/rollerball/
	cd web && npm run build
	cp -r web/src build/rollerball/websrc/src
	cp -r web/public build/rollerball/websrc/public
	cp web/README.md web/index.html web/package-lock.json web/package.json web/vite.config.js build/rollerball/websrc/
	cp -r web/dist build/rollerball/web
	cd build && zip -r rollerball.zip rollerball

dbg_frontend: src/debug_frontend.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) src/server.cpp src/debug_frontend.cpp -o bin/debug_frontend

dbg_uciws: src/debug_uciws.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) src/server.cpp src/uciws.cpp src/debug_uciws.cpp -o bin/debug_uciws

dbg_board: src/debug_board.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) src/board.cpp src/debug_board.cpp -o bin/debug_board

dbg_moves: src/debug_moves.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) src/bdata.cpp src/butils.cpp src/board.cpp src/debug_moves.cpp -o bin/dbg_moves

clean:
	rm bin/*
