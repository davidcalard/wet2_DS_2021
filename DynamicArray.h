#ifndef WET2_DYNAMICARRAY_H
#define WET2_DYNAMICARRAY_H

#define DEFAULT_SIZE 16

enum ArrayStatus {AS_SUCCESS,AS_FAIL};

/*------------------------------------------------------------------------------
-----------------------------------DynamicArray---------------------------------
------------------------------------------------------------------------------*/

template<class Data>
class DynamicArray
{
    Data** array = nullptr;
    int size = DEFAULT_SIZE;
    int usedSize = 0;
public:
    DynamicArray();
    ~DynamicArray();

    ArrayStatus expand();
    ArrayStatus insert(Data* data,int& n);
    ArrayStatus insertNext(Data* data);

    int getSize(){return usedSize;}

    Data operator[](int n);

};


/*------------------------------------------------------------------------------
----------------------------DynamicArray-Implementation-------------------------
------------------------------------------------------------------------------*/

template<class Data>
DynamicArray<Data>::DynamicArray() {
    array = new Data[size];
    for(int i=0;i<size;i++)
        array[i] = nullptr;
}

/*----------------------------------------------------------------------------*/

template<class Data>
DynamicArray<Data>::~DynamicArray() {
    delete[] array;
}

/*----------------------------------------------------------------------------*/

template<class Data>
ArrayStatus DynamicArray<Data>::expand() { //assume array is not nullptr
    Data *temp = new Data[2 * size];
    if (!temp) return AS_FAIL;
    else {
        for (int i = 0; i < usedSize; i++) {
            temp[i] = array[i];
        }
        delete [] array;
        return AS_SUCCESS;
    }
}
/*----------------------------------------------------------------------------*/

template<class Data>
Data DynamicArray<Data>::operator[](int n) {
    return n < size ? array[n] : nullptr;
}

template<class Data>
ArrayStatus DynamicArray<Data>::insert(Data* data, int &n) {
    if(size<=n) return AS_FAIL;
    else{
        if(usedSize >= size){
            if(expand() == AS_FAIL) return AS_FAIL;
        }
        array[n] = data;
        usedSize++;
        return AS_SUCCESS;
    }
}

template<class Data>
ArrayStatus DynamicArray<Data>::insertNext(Data* data) {
    return insert(data,usedSize);
}

#endif //WET2_DYNAMICARRAY_H