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
            Vertex pXY(1,1,0);
            Vertex pXYZ(1,1,1);
            Vertex pXYZ2(0.5,0.5,0.5);
            //=========Basic Operations Tests
            //norm tests
            std::cout << "Norm testing..." << std::endl;
            assert( norm(oX) == 1 );
            std::cout << "Norm tests passed" << std::endl;
            //dot tests
            std::cout << "Dot product testing..." << std::endl;
            assert( dot(oX,oY) == 0 );
            assert( dot(oXY,oY) == 1);
            std::cout << "Dot product tests passed" << std::endl;
            //Vector Creation test
            std::cout << "Vector operators testing..." << std::endl;
            Vector vec = pXYZ - p;
            assert(vec.x == 1 && vec.y == 1 && vec.z == 1);
            std::cout << "Vector operators tests passed" << std::endl;
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
            assert(getArea(p,pX,pY) == 0.5);
            std::cout << "getArea tests passed" << std::endl;
            //Barycentric operators
            std::cout << "Barycentric operators testing..." << std::endl;
            Vertex bar = 0.5 * p + 0.5 * pX;
            assert(bar.x() == 0.5 && bar.y() == 0 && bar.z() == 0); 
            bar = 0.5 * p + pXYZ * 0.5;
            assert(bar.x() == 0.5 && bar.y() == 0.5 && bar.z() == 0.5); 
            std::cout << "Barycentric operators tests passed..." << std::endl;
            //=========Delaunay Tests
            std::cout << "Delaunay function testing..." << std::endl;
            //Vertex delCenter = getCircumCenter(2*pX, pXY*2,pY);
            //Found center to be visually correct at [1.25][1.0][0.0]
            // assert(norm(2*pX - delCenter) == norm(2*pXY - delCenter) && norm(2*pX - delCenter) == norm(pY - delCenter));
            assert(isInCircle(2*pX, pXY*2,pY,pXY));
            assert(!isInCircle(2*pX, pXY*2,pY,2.1*pXY));
            std::cout << "Delaunay functions test passed" << std::endl;
            
        }
        void run(){
            testVectors();
        }
};
