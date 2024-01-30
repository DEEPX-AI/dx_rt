// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/util.h"
#include "dxrt/datatype.h"
#include "dxrt/inference_engine.h"
#include <algorithm>

using namespace std;
namespace dxrt
{
unsigned int RandomValue()
{
    random_device rd;
    mt19937_64 eng(rd());
    uniform_int_distribution<unsigned int> distr;
    return distr(eng);
}

vector<int> RandomSequence(int n)
{    
    vector<int> v(n);
    for(int i=0;i<n;i++)
        v[i] = i;
    random_shuffle(v.begin(), v.end());
    cout << __func__ << " : ";
    for(int i=0;i<n;i++)
        cout << dec << v[i] << " ";
    cout << endl;
    return v;
}

vector<string> StringSplit(string s, string divid)
{
	vector<string> v;
    if(!s.empty())
    {
        char* c = strtok((char*)s.c_str(), divid.c_str());
        while (c) {
            v.push_back(c);
            c = strtok(NULL, divid.c_str());
        }
    }
    else
    {
        v.emplace_back("");
    }
	return v;
}

int DataCompare(void *d1, void *d2, int size)
{
    int ret = -1;
    if(size<sizeof(uint64_t))
    {
        uint64_t buf1 = 0, buf2 = 0;
        memcpy((void*)&buf1, d1, size);
        memcpy((void*)&buf2, d2, size);
        if(memcmp((void*)&buf1, (void*)&buf2, size)==0)
            ret = 0;
        else
            ret = -1;
    }
    else
    {
        uint64_t *buf1 = (uint64_t*)d1;
        uint64_t *buf2 = (uint64_t*)d2;
        for(int i=0;i<size/sizeof(uint64_t);i++)
        {
            if(buf1[i]!=buf2[i])
            {
                // cout << hex << i*sizeof(uint64_t) << ": " << buf1[i] << dec << endl;
                if(i==0)
                    return -1;
                else
                    return i*sizeof(uint64_t);
            }
        }
        ret = 0;
    }    
    return ret;
}

int DataCompare(string f1, string f2)
{
    ifstream f_in1(f1, ios::binary|ifstream::ate);
    ifstream f_in2(f2, ios::binary|ifstream::ate);
    int ret;

    ifstream::pos_type size_f1 = f_in1.tellg();
    ifstream::pos_type size_f2 = f_in2.tellg();

    DXRT_ASSERT(size_f1==size_f2, "different data size to compare");
    void *buf1, *buf2;
    buf1 = dxrt::MemAlloc(size_f1);
    buf2 = dxrt::MemAlloc(size_f2);
    f_in1.seekg(0, ifstream::beg);
    f_in2.seekg(0, ifstream::beg);
    f_in1.read((char*)buf1, size_f1);
    f_in2.read((char*)buf2, size_f2);
    ret = memcmp(buf1, buf2, size_f1);
    cout << "    " << f1 << endl << "    vs " << f2 << " : " << ret << ((ret==0)?(" [PASS]"):(" [FAIL]")) << endl;
    f_in1.close();
    f_in2.close();
    dxrt::MemFree(&buf1);
    dxrt::MemFree(&buf2);

    return ret;
}
///////////////////// Data Compare Logic using npu param file information /////////////////////////
int GetDataSize(deepx_rmapinfo::DataType dType)
{
    int size = 0;
    switch(dType){
        case deepx_rmapinfo::DataType::UINT8  :
        case deepx_rmapinfo::DataType::INT8   :
            size = 1;
            break;
        case deepx_rmapinfo::DataType::INT16  :
        case deepx_rmapinfo::DataType::UINT16 :
            size = 2;
            break;
        case deepx_rmapinfo::DataType::UINT32 :
        case deepx_rmapinfo::DataType::INT32  :
        case deepx_rmapinfo::DataType::FLOAT32:
            size = 4;
            break;
        case DataType::BBOX :
            size = 32;
            break;
        case DataType::FACE :
            size = 64;
            break;
        case DataType::POSE :
            size = 256;
            break;
        default:
            cout << "Unwanted Data Type is inserted in GetDataSize." << dType << endl;
            exit(0);
    }
    return size;
}
vector<int> GetShape(deepx_rmapinfo::Shapes shape, deepx_rmapinfo::DataFormat dFormat)
{
    if (dFormat == deepx_rmapinfo::DataFormat::NHWd)
    {
        vector<int> shapes{shape.shape(0), 1, shape.shape(1), shape.shape(2)};
        return shapes;
    }
    else
    {
        // deepx_rmapinfo::DataFormat::NHWCd
        vector<int> shapes{shape.shape(0), shape.shape(3), shape.shape(1), shape.shape(2)};
        return shapes;
    }
}

int GetBaseStride(deepx_rmapinfo::DataType dType, deepx_rmapinfo::DataFormat dFormat, int shapeC, int *sLoop)
{
    int stride = 0;
    if (dFormat == deepx_rmapinfo::DataFormat::NHWd)
    {
        stride = 2;
        return stride;
    }

    switch(dType){
        /*64units compare mode of channel shape*/
        case deepx_rmapinfo::DataType::UINT8  :
        case deepx_rmapinfo::DataType::INT8   :
            stride = 64;
            *sLoop = ceil(shapeC/64.);
            break;
        /*Straigt compare mode of channel shape*/
        case deepx_rmapinfo::DataType::INT16  :
        case deepx_rmapinfo::DataType::UINT16 :
            stride = 32;
            break;
        case deepx_rmapinfo::DataType::UINT32 :
        case deepx_rmapinfo::DataType::INT32  :
        case deepx_rmapinfo::DataType::FLOAT32:
            stride = 256;
            break;
        default:
            cout << "Unwanted Data Type is inserted in GetStride." << dType << endl;
            exit(0);
    }
    return stride;
}

template< typename T >
std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << "0x" 
         << std::setfill ('0') << std::setw(sizeof(T)*2) 
         << std::hex << i;
  return stream.str();
}

int CompareBuf(char *DataCompareBuf1, char *DataCompareBuf2, deepx_rmapinfo::RegisterInfoDatabase Cfg, string *logBuf, int log_en, int type)
{
    int ret = 0;
    if(Cfg.layers().layer_size()<=0)
    {
        ret = memcmp(DataCompareBuf1, DataCompareBuf2, Cfg.outputs().memory().size());
        return ret;
    }
    /*Data Compare
        type 0 : all layers
        type 1 : output layers
    */
    if(type==0)
    {
        uint64_t base = Cfg.outputs().memory().offset();
        uint32_t num_layers = Cfg.counts().layer();

        for (int l=0; l<num_layers; l++)
        {
            deepx_rmapinfo::Layer layer = Cfg.layers().layer(l);
            deepx_rmapinfo::Shapes shape = Cfg.layers().layer(l).output().shapes();
            /* Check whether NPU Output is writted to DDR or not */
            int base_off = layer.memory().size() == 0 ? -1 : layer.memory().offset() - base;
            int sLoop = 1;
            int dSize = GetDataSize((deepx_rmapinfo::DataType)Cfg.layers().layer(l).output().type());
            int baseS = GetBaseStride((deepx_rmapinfo::DataType)Cfg.layers().layer(l).output().type(), deepx_rmapinfo::DataFormat::NHWCd, shape.shape(1), &sLoop);
            int stride = (baseS >= shape.shape(1)*dSize) ? baseS : ceil(shape.shape(1)*dSize/(float)baseS)*baseS;
            int cmpSize = shape.shape(1) * dSize;
            int cmpSize_rest = cmpSize; /*only user in 1 byte data size*/
            // printf("[DBG] layer %d, name %s, BaseOffset:%d, shape:[%d,%d,%d,%d], dSize:%d, stride:%d, cmpSize:%d, baseS:%d sLoop:%d\n", \
            //             l, layer.memory().name().c_str(), base_off, \
            //             shape.shape(0), shape.shape(1),shape.shape(2), shape.shape(3),\
            //             dSize, stride, cmpSize, baseS, sLoop);
            if (base_off >= 0) {
                for (int il=0; il<sLoop; il++)
                {
                    if (dSize == 1) { /*Only 1 byte data size*/
                        cmpSize = (cmpSize_rest/64. > 1) ? 64 : cmpSize_rest;
                        if (cmpSize == 64)
                            cmpSize_rest = cmpSize_rest - 64;
                        stride = 64;
                        /* In case the dummy is not existed on memory, - only shape1 : 32 */
                        if (
                            (shape.shape(1) == 32) &&
                            (layer.memory().size() == shape.shape(1)*shape.shape(2)*shape.shape(3))
                            )
                            stride = 32;
                    }
                    // printf("inner loop : %d, cmpSize : %d, stride : %d\n", il, cmpSize, stride);
                    for (int h=0; h<shape.shape(2); h++)
                    {
                        for (int w=0; w<shape.shape(3); w++)
                        {
                            uint32_t idx = base_off + il*shape.shape(3)*shape.shape(2)*stride + h*shape.shape(3)*stride + w*stride;
                            ret = memcmp(   &DataCompareBuf1[ idx ],
                                            &DataCompareBuf2[ idx ],
                                            cmpSize);
                            if (ret != 0)
                            {
                                int tile_num = layer.number().tile();
                                int layer_num = layer.number().layer();
                                string failInfo = "[Fail Layer Info] Name:" + layer.memory().name() + \
                                                    ", L:" + to_string(layer_num) + ", T:" + to_string(tile_num) + ", H:" + to_string(h) + ", W:" + to_string(w) + \
                                                    ", Fail Offset:" + int_to_hex(idx);
                                // failInfo.append("num_layers:" + to_string(l) + \
                                //                 ", BaseOffset:" + to_string(base_off) + \
                                //                 ", shape:["+to_string(shape.shape(0))+","+\
                                //                             to_string(shape.shape(1))+","+\
                                //                             to_string(shape.shape(2))+","+\
                                //                             to_string(shape.shape(3))+"]" + \
                                //                 ", dSize:" + to_string(dSize) + \
                                //                 ", stride:" + to_string(stride) + \
                                //                 ", cmpSize:" + to_string(cmpSize) + '\n');
                                for (int j=0;j<cmpSize;j++)
                                {
                                    if(DataCompareBuf1[idx+j] != DataCompareBuf2[idx+j])
                                    {
                                        uint32_t a = (uint32_t)(unsigned char)DataCompareBuf1[idx+j];
                                        uint32_t b = (uint32_t)(unsigned char)DataCompareBuf2[idx+j];
                                        failInfo.append(
                                            ", [" + int_to_hex(idx+j) + "] : " + int_to_hex(a) + " vs " + int_to_hex(b)
                                        );
                                        break;
                                    }
                                }
                                failInfo.append("\n");
                                logBuf->append(failInfo);
                                cout << failInfo;

                                cout << "Buffer1 data..." << endl;
                                uint32_t *ptr = (uint32_t*)&DataCompareBuf1[idx];
                                for(int j=0;j<cmpSize/sizeof(uint32_t);j++)
                                {
                                    cout << hex << ptr[j] << " ";
                                }
                                cout << endl;
                                cout << "Buffer2 data..." << endl;
                                ptr = (uint32_t*)&DataCompareBuf2[idx];
                                for(int j=0;j<cmpSize/sizeof(uint32_t);j++)
                                {
                                    cout << hex << ptr[j] << " ";
                                }
                                cout << endl;
                                goto fail;
                            }
                        }
                    }
                }
            } else {
                if (log_en)
                    cout << "The data of layer are not saved in RAM (Layer:" << dec << l << ")" << endl;
            }
        }
    }
    else if(type==1)
    {
        uint64_t base;
        uint32_t num_outputs = Cfg.outputs().outputlist().output_size();
        vector<uint64_t> offsets;        
        for(int i=0; i<num_outputs; i++)
        {
            offsets.emplace_back(Cfg.outputs().outputlist().output(i).memory().offset());
        }
        base = offsets.front();
        for(auto offset:offsets)
        {
            base = min(offset, base);
        }
        for(int i=0; i<num_outputs; i++)
        {
            vector<int> shapes = GetShape(Cfg.outputs().outputlist().output(i).shapes(), (deepx_rmapinfo::DataFormat)Cfg.outputs().outputlist().output(i).format());
            int base_off = Cfg.outputs().outputlist().output(i).memory().offset() - base;
            int sLoop = 1;
            int dSize = GetDataSize((deepx_rmapinfo::DataType)Cfg.outputs().outputlist().output(i).type());
            int baseS = GetBaseStride((deepx_rmapinfo::DataType)Cfg.outputs().outputlist().output(i).type(), (deepx_rmapinfo::DataFormat)Cfg.outputs().outputlist().output(i).format(), shapes[1], &sLoop);
            int stride = (baseS >= shapes[1]*dSize) ? baseS : ceil(shapes[1]*dSize/(float)baseS)*baseS;
            int cmpSize = shapes[1] * dSize;
            int cmpSize_rest = cmpSize; /*only user in 1 byte data size*/
            // printf("[DBG] output %d, name %s, BaseOffset:%d, shape:[%d,%d,%d,%d], dSize:%d, stride:%d, cmpSize:%d, baseS:%d sLoop:%d\n", \
            //             i, Cfg.outputs().outputlist().output(i).name().c_str(), base_off, \
            //             shapes[0], shapes[1], shapes[2], shapes[3],\
            //             dSize, stride, cmpSize, baseS, sLoop);
            if (base_off >= 0) {
                for (int il=0; il<sLoop; il++)
                {
                    if (dSize == 1) { /*Only 1 byte data size*/
                        cmpSize = (cmpSize_rest/64. > 1) ? 64 : cmpSize_rest;
                        if (cmpSize == 64)
                            cmpSize_rest = cmpSize_rest - 64;
                        stride = 64;
                    }
                    // printf("inner loop : %d, cmpSize : %d, stride : %d\n", il, cmpSize, stride);
                    for (int h=0; h<shapes[2]; h++)
                    {
                        for (int w=0; w<shapes[3]; w++)
                        {
                            uint32_t idx = base_off + il*shapes[3]*shapes[2]*stride + h*shapes[3]*stride + w*stride;
                            ret = memcmp(   &DataCompareBuf1[ idx ],
                                            &DataCompareBuf2[ idx ],
                                            cmpSize);
                            // cout << hex << (uint32_t)DataCompareBuf1[ idx ] << ", " << (uint32_t)DataCompareBuf2[ idx ] << endl;
                            if (ret != 0)
                            {
                                // int tile_num = layer.number().tile();
                                // int layer_num = layer.number().layer();
                                string failInfo = "[Fail Layer Info] Name:" + Cfg.outputs().outputlist().output(i).name() + \
                                                    ", H:" + to_string(h) + ", W:" + to_string(w) + \
                                                    ", Fail Offset:" + to_string(idx) + "\n";
                                failInfo.append("output index:" + to_string(i) + \
                                                ", BaseOffset:" + to_string(base_off) + \
                                                ", shape:["+to_string(shapes[0])+","+\
                                                            to_string(shapes[1])+","+\
                                                            to_string(shapes[2])+","+\
                                                            to_string(shapes[3])+"]" + \
                                                ", dSize:" + to_string(dSize) + \
                                                ", stride:" + to_string(stride) + \
                                                ", cmpSize:" + to_string(cmpSize) + '\n');
                                (*logBuf).append(failInfo);
                                cout << failInfo;

                                cout << "Buffer1 data..." << endl;
                                uint32_t *ptr = (uint32_t*)&DataCompareBuf1[idx];
                                for(int j=0;j<cmpSize/sizeof(uint32_t);j++)
                                {
                                    cout << hex << ptr[j] << " ";
                                }
                                cout << endl;
                                cout << "Buffer2 data..." << endl;
                                ptr = (uint32_t*)&DataCompareBuf2[idx];
                                for(int j=0;j<cmpSize/sizeof(uint32_t);j++)
                                {
                                    cout << hex << ptr[j] << " ";
                                }
                                cout << endl;
                                goto fail;
                            }
                        }
                    }
                }
            } else {
                // if (log_en)
                //     cout << "The data of layer are not saved in RAM (Layer:" << dec << l << ")" << endl;
            }
        }
    }
fail:
    return ret;
}
// int DataCompare(string f1, string f2, string path, string *logBuf, int log_en, int type)
// {
//     deepx_rmapinfo::RegisterInfoDatabase cfg = dxrt::LoadNpuParam(path+"/"+NPU_PARAM_FILE);
//     // cfg.PrintDebugString();
//     return DataCompare(f1, f2, cfg, logBuf, log_en, type);
// }
int DataCompare(string f1, string f2, deepx_rmapinfo::RegisterInfoDatabase Cfg, string *logBuf, int log_en, int type)
{
    LOG_DXRT_DBG << endl;
    if(Cfg.layers().layer_size()<=0)
    {
        return DataCompare(f1, f2);
    }
    ifstream f_in1(f1, ios::binary|ifstream::ate);
    ifstream f_in2(f2, ios::binary|ifstream::ate);
    int ret;

    ifstream::pos_type size_f1 = f_in1.tellg();
    ifstream::pos_type size_f2 = f_in2.tellg();

    DXRT_ASSERT(size_f1==size_f2, "different data size to compare");
    void *buf1, *buf2;
    buf1 = dxrt::MemAlloc(size_f1);
    buf2 = dxrt::MemAlloc(size_f2);

    f_in1.seekg(0, ifstream::beg);
    f_in2.seekg(0, ifstream::beg);

    f_in1.read((char*)buf1, size_f1);
    f_in2.read((char*)buf2, size_f2);
    ret = CompareBuf((char*)buf1, (char*)buf2, Cfg, logBuf, log_en, type);
    cout << "    " << f1 << endl << "    vs " << f2 << " : " << ret << ((ret==0)?(" [PASS]"):(" [FAIL]")) << endl;
    f_in1.close();
    f_in2.close();
    dxrt::MemFree(&buf1);
    dxrt::MemFree(&buf2);

    return ret;
}
int DataCompare(char *f1, char *f2, deepx_rmapinfo::RegisterInfoDatabase Cfg, string *logBuf, int log_en, int type)
{
    int ret = CompareBuf(f1, f2, Cfg, logBuf, log_en, type);
    LOG_DXRT_DBG << "    Data Compare Result : " << ((ret==0)?(" [PASS]"):(" [FAIL]")) << endl;
    if(log_en)
        cout << "    Data Compare Result : " << ((ret==0)?(" [PASS]"):(" [FAIL]")) << endl;
    return ret;
}
int DataFromFile(string f, void *d)
{
    LOG_DXRT_DBG << f << " -> " << d << endl;
    ifstream in(f, ifstream::binary);
    if(in)
    {
        in.clear();
		in.seekg(0, in.end);
		unsigned int size = (int)in.tellg();
		in.seekg(0, in.beg);
        in.read((char*)d, size);
        in.close();
        return size;
    }
    return 0;
}
void DataFromFile(string f, void *d, unsigned int size)
{
    FILE *fp;
    fp = fopen(f.c_str(), "rb");
    fread(d, size, 1, fp);
    fclose(fp);
}
uint32_t SizeFromFile(string f)
{
    ifstream in(f, ifstream::binary);
    uint32_t size = 0;
    if(in)
    {
		in.seekg(0, in.end);
		size = (int)in.tellg();
        in.close();
    }
    return size;
}
void DataDumpBin(string filename, void *data, unsigned int size)
{
    FILE *fp;
    fp = fopen(filename.c_str(), "wb");
    fwrite(data, size, 1, fp);
    fclose(fp);
}
vector<string> GetFileList(string dir)
{
    vector<string> v;
    DIR* p = opendir(dir.c_str());
    struct dirent *dp;
    while((dp = readdir(p)) != NULL)
    {
        // cout << dp->d_name << "===" << endl;
        if(strcmp(dp->d_name, ".")!=0 && strcmp(dp->d_name, "..")!=0)
            v.emplace_back(dp->d_name);
    }
    closedir(p);
    return v;
}

uint64_t GetAlign64(uint64_t size)
{
    int remainder = size % 64;
    if (remainder != 0) {
        size += 64 - remainder;
    }
    return size;
}

template<typename T>
int DataComparePpu(T* d1, T* d2, int size)
{
    int ret = -1;
    if(size<32) return 0;
    for(int i=0;i<size/sizeof(T);i++)
    {
        // cout << i << ", " << d1[i].score << ", " << d1[i].x << ", " << d1[i].y << ", " << d1[i].w << ", " << d1[i].h << endl;
        // cout << i << ", " << d2[i].score << ", " << d2[i].x << ", " << d2[i].y << ", " << d2[i].w << ", " << d2[i].h << endl;
        if( memcmp(&d1[i], &d2[i], sizeof(T)) != 0)
        {
            if(i==0)
            {
                return -1;
            }
            else
            {
                return i*sizeof(T);
            }
        }
        else
        {
            ret = 0;
        }
    }
    return ret;
}
int(*compareBoundingBox)(DeviceBoundingBox_t*, DeviceBoundingBox_t*, int) = DataComparePpu<DeviceBoundingBox_t>;
int(*compareFace)(DeviceFace_t*, DeviceFace_t*, int) = DataComparePpu<DeviceFace_t>;
int(*comparePose)(DevicePose_t*, DevicePose_t*, int) = DataComparePpu<DevicePose_t>;
int(*comparePpuDataFunctions[])(void*, void*, int)  = {
    reinterpret_cast<int(*)(void*, void*, int)>(compareBoundingBox),
    reinterpret_cast<int(*)(void*, void*, int)>(compareFace),
    reinterpret_cast<int(*)(void*, void*, int)>(comparePose),
};
int DataCompare(DataType type, void *d1, void *d2, int size)
{
    int ret = -1;
    if(type>=DataType::BBOX && type<=DataType::POSE)
    {
        ret = comparePpuDataFunctions[ type - DataType::BBOX ](d1, d2, size);
    }    
    return ret;
}
void* MemAlloc(size_t size, size_t align, int value)
{
    void *mem = NULL;
    int rc = posix_memalign((void**)&mem, align, size);
    if(rc==EINVAL)
    {
        cout << "Error: posix_memalign returned EINVAL." << endl;
        return mem;
    }
    else if(rc==ENOMEM)
    {
        cout << "Error: posix_memalign returned ENOMEM." << endl;
        return mem;
    }
    if (value != 0)
    {
        LOG_DBG( "Default value of allocation memory:"<< value );
        memset(mem, value, size);
    }
    if(!mem)
    {
        LOG_DBG( "failed to allocate memory: " << size << ", " << align );
    }
    return mem;
}
void MemFree(void **p)
{
    if(*p!=nullptr)
    {
        free(*p);
        *p = nullptr;
    }
}

ostream& operator<<(ostream& os, const Processor& processor)
{
    switch (processor)
    {
        case NPU:
            os << "NPU";
            break;
        case CPU:
            os << "CPU";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}

} // namespace dxrt