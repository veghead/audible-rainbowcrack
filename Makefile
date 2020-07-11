
all: audible-crack

audible-crack: *.h *.cpp
	g++ Public.cpp ChainWalkContext.cpp HashRoutine.cpp HashSet.cpp MemoryPool.cpp ChainWalkSet.cpp CrackEngine.cpp RainbowCrack.cpp sha1.cpp -O3 -o audible-crack

clean:
	rm -f audible-crack
