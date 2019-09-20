#include <iostream>
#include <QMap>

class SegmentMemory{
    class SegmentMemoryKey{
    public:
        int vertexIndex1;
        int vertexIndex2;
        SegmentMemoryKey(int index1, int index2) {
            if(index1 < index2){
                vertexIndex1 = index1;
                vertexIndex2 = index2;
            } else {
                vertexIndex1 = index2;
                vertexIndex2 = index1;
            }
        }

        bool operator==(const SegmentMemoryKey& cmp) const{
            return (this->vertexIndex1==cmp.vertexIndex2&&this->vertexIndex2==cmp.vertexIndex1);
        }

        bool operator<(const SegmentMemoryKey& cmp) const{
            return ((this->vertexIndex1<cmp.vertexIndex1)
                    ||(this->vertexIndex1==cmp.vertexIndex1&&this->vertexIndex2<cmp.vertexIndex2));
        }

    };

    class SegmentMemoryData{
    public:
        int faceIndex;
        int vertexInFaceIndex;
        SegmentMemoryData(int fIndex, int vertexIndex)
            : faceIndex(fIndex), vertexInFaceIndex(vertexIndex){}
    };
public:
    QMap<SegmentMemoryKey, SegmentMemoryData> hashMap;

    void addSegment(int vI1, int vI2, int fI, int vIFI){
        hashMap.insert(SegmentMemoryKey(vI1, vI2), SegmentMemoryData(fI, vIFI));
    }

    void deleteSegment(const int cmp[2]){
        SegmentMemoryKey toDelete(cmp[0], cmp[1]);
        QMap<SegmentMemoryKey, SegmentMemoryData>::iterator it = hashMap.find(toDelete);
        hashMap.erase(it);
    }

    bool contain(const int cmp[2]){
        return hashMap.contains(SegmentMemoryKey(cmp[0], cmp[1]));
    }

    int faceIndex(const int cmp[2]){
        return (hashMap.find(SegmentMemoryKey(cmp[0], cmp[1]))).value().faceIndex;
    }

    int vertexInFaceIndex(const int cmp[2]){
        return (hashMap.find(SegmentMemoryKey(cmp[0], cmp[1]))).value().vertexInFaceIndex;
    }

    void print(){
        auto it = hashMap.keyBegin();
        for(auto h : hashMap){
            std::cout<<"("<<it->vertexIndex1<<", "<<it->vertexIndex2<<") ("
                    <<h.faceIndex<<", " << ")\n";
            it++;
        }
    }
};

