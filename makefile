all: 3-A.out 3-B.out

3-A.out: 3-A.cpp
	$(CXX) 3-A.cpp -std=c++17 -o 3-A.out

3-B.out: 3-B.cpp
	$(CXX) 3-B.cpp -std=c++17 -o 3-B.out

clean:
	rm 3-A.out 3-B.out