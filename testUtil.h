#include "utils.h"
#include <cassert>
#include <iostream>
class TestUtil{
    public:
        TestUtil(){}
        void testVectors(){
            std::cout << "Vector testing..." << std::endl;
            Vector o(0,0,0);
            Vector oX(1,0,0);
            Vector oY(0,1,0);
            Vector oZ(0,0,1);
            Vector oXY(1,1,0);
            Vector oYY(0,1,1);
            Vertex p(0,0,0);
            Vertex pX(1,0,0);
            Vertex pY(0,1,0);
            //dot tests
            std::cout << "Dot product testing..." << std::endl;
            assert( dot(oX,oY) == 0 );
            assert( dot(oXY,oY) == 1);
            std::cout << "Dot product test passed" << std::endl;
            //cross tests
            std::cout << "Cross product testing..." << std::endl;
            assert( cross(oX,oY).z == 1 );
            std::cout << "Cross product test passed" << std::endl;
            //cosine testing
            std::cout << "getCos / getSin testing..." << std::endl;
            assert( getCos(oX,oY) == 0 );
            std::cout << getSin(oX,oY);
            assert( getSin(oX,oY) == 1 );
            std::cout << "getCos / getSin test passed" << std::endl;
            //orientationTest
            std::cout << "Orientation testing..." << std::endl;
            assert(orientation(p,pX,pY) > 0);
            assert(orientation(p,pY,pX) < 0);
            std::cout << "Orientation test passed" << std::endl;
        }
        void run(){
            testVectors();
        }
};
