#pragma once
#include <cstddef> 
#include<string>

class MmapArena{
    public:
    MmapArena(const std::string& filename,std::size_t size);
    ~MmapArena();

    MmapArena(const MmapArena&) = delete;
    MmapArena& operator=(const MmapArena&)=delete;

    MmapArena(MmapArena&& other) noexcept;
    MmapArena& operator=(MmapArena&& other) noexcept; //&& is r value reference

    void*  data();

    const void*  data() const;
    std::size_t size() const;

    void grow(std::size_t new_size);

    void flush();

    private:

    std::string filename_;

    std::size_t size_;

    void* data_;
    
    int fd_;

    void unmap();
    void map();
    

};
