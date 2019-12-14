#include <iostream>
#include <QMap>

class SegmentMapKey{
public:
    int vertexIndex1;
    int vertexIndex2;
    SegmentMapKey(){
        vertexIndex1 = 0;
        vertexIndex2 = 0;
    }

    SegmentMapKey(int index1, int index2) {
        if(index1 < index2){
            vertexIndex1 = index1;
            vertexIndex2 = index2;
        } else {
            vertexIndex1 = index2;
            vertexIndex2 = index1;
        }
    }
    SegmentMapKey(const SegmentMapKey & t){
        vertexIndex1=t.vertexIndex1;
        vertexIndex2=t.vertexIndex2;
    }

    bool operator==(const SegmentMapKey& cmp) const{
        return (this->vertexIndex1==cmp.vertexIndex2&&this->vertexIndex2==cmp.vertexIndex1);
    }

    bool operator<(const SegmentMapKey& cmp) const{
        return ((this->vertexIndex1<cmp.vertexIndex1)
                ||(this->vertexIndex1==cmp.vertexIndex1&&this->vertexIndex2<cmp.vertexIndex2));
    }

};

class SegmentMapNeighbor{
public:

    class SegmentMapNeighborData{
    public:
        int faceIndex;
        int vertexInFaceIndex;
        SegmentMapNeighborData(int fIndex, int vertexIndex)
            : faceIndex(fIndex), vertexInFaceIndex(vertexIndex){}

        bool operator==(const SegmentMapNeighborData& cmp) const{
            return this->faceIndex==cmp.faceIndex
                    &&this->vertexInFaceIndex==cmp.vertexInFaceIndex;
        }
    };


public:
    QMap<SegmentMapKey, SegmentMapNeighborData> hashMap;

    void addSegment(int vI1, int vI2, int fI, int vIFI){
        hashMap.insert(SegmentMapKey(vI1, vI2), SegmentMapNeighborData(fI, vIFI));
    }

    void deleteSegment(const int cmp[2]){
        SegmentMapKey toDelete(cmp[0], cmp[1]);
        QMap<SegmentMapKey, SegmentMapNeighborData>::iterator it = hashMap.find(toDelete);
        hashMap.erase(it);
    }

    bool contain(const int cmp[2]){
        return hashMap.contains(SegmentMapKey(cmp[0], cmp[1]));
    }

    int faceIndex(const int cmp[2]){
        return (hashMap.find(SegmentMapKey(cmp[0], cmp[1]))).value().faceIndex;
    }

    int vertexInFaceIndex(const int cmp[2]){
        return (hashMap.find(SegmentMapKey(cmp[0], cmp[1]))).value().vertexInFaceIndex;
    }

    bool isEmpty(){ return hashMap.isEmpty(); }

    int size(){ return hashMap.size(); }

    /*void print(){
        auto it = hashMap.keyBegin();
        for(auto h : hashMap){
            std::cout<<"("<<it->vertexIndex1<<", "<<it->vertexIndex2<<") ("
                    <<h.faceIndex<<", " << ")\n";
            it++;
        }
    }*/
};

class SegmentMapSimplify{
public:
    class SegmentMapSimplifyData{
    public:
        double dist;
        int faceId1;
        int faceId2;
        SegmentMapSimplifyData(double distance, int fId1, int fId2){
            dist = distance;
            faceId1 = fId1;
            faceId2 = fId2;
        }
    };

public:
    QMap<SegmentMapKey, SegmentMapSimplifyData> hashMap;

    void print(){
        auto it = hashMap.keyBegin();
        for(auto h : hashMap){
            printf("Segment (%d, %d) de distance %f, pour les faces %d, %d.\n",
                   it->vertexIndex1, it->vertexIndex2, h.dist, h.faceId1, h.faceId2);
            fflush(stdout);

            it++;
        }
    }
};
