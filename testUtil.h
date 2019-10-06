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
            //norm tests
            std::cout << "Norm testing..." << std::endl;
            assert( norm(oX) == 1 );
            std::cout << "Norm tests passed" << std::endl;
            //dot tests
            std::cout << "Dot product testing..." << std::endl;
            assert( dot(oX,oY) == 0 );
            assert( dot(oXY,oY) == 1);
            std::cout << "Dot product tests passed" << std::endl;
            //cross tests
            std::cout << "Cross product testing..." << std::endl;
            assert( cross(oX,oY).z == 1 );
            std::cout << "Cross product test passed" << std::endl;
            //cosine testing
            std::cout << "getCos / getSin testing..." << std::endl;
            assert( getCos(oX,oY) == 0 );
            assert( getSin(oX,oY) == 1 );
            std::cout << "getCos / getSin tests passed" << std::endl;
            //orientation Tests
            std::cout << "Orientation testing..." << std::endl;
            assert(orientation(p,pX,pY) > 0);
            assert(orientation(p,pY,pX) < 0);
            std::cout << "Orientation tests passed" << std::endl;
            //area Tests
            std::cout << "getArea testing..." << std::endl;
            std::cout << "area = " << getArea(p,pX,pY);
            assert(getArea(p,pX,pY) == 0.5);
            std::cout << "getArea tests passed" << std::endl;
        }
        void run(){
            testVectors();
        }
};
