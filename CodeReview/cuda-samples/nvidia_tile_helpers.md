
在阅读tile_helper相关的header file时（位于cuda libararies:include\cooperative_groups\details\helpers.h）

```cpp
    template <unsigned int TileCount, unsigned int TileMask, unsigned int LaneMask, unsigned int ShiftCount>
    struct _tile_helpers{
        _CG_STATIC_CONST_DECL unsigned int tileCount = TileCount;
        _CG_STATIC_CONST_DECL unsigned int tileMask = TileMask;
        _CG_STATIC_CONST_DECL unsigned int laneMask = LaneMask;
        _CG_STATIC_CONST_DECL unsigned int shiftCount = ShiftCount;
    };

    template <unsigned int> struct tile_helpers;
    template <> struct tile_helpers<32> : public _tile_helpers<1,  0xFFFFFFFF, 0x1F, 5> {};
    template <> struct tile_helpers<16> : public _tile_helpers<2,  0x0000FFFF, 0x0F, 4> {};
    template <> struct tile_helpers<8>  : public _tile_helpers<4,  0x000000FF, 0x07, 3> {};
    template <> struct tile_helpers<4>  : public _tile_helpers<8,  0x0000000F, 0x03, 2> {};
    template <> struct tile_helpers<2>  : public _tile_helpers<16, 0x00000003, 0x01, 1> {};
    template <> struct tile_helpers<1>  : public _tile_helpers<32, 0x00000001, 0x00, 0> {};

    typedef details::tile::tile_helpers<Size> th;
#ifdef _CG_CPP11_FEATURES
    static_assert(details::_is_valid_single_warp_tile<Size>::value, "Size must be one of 1/2/4/8/16/32");
#else
    typedef typename details::verify_thread_block_tile_size<Size>::OK valid;
#endif
    using __static_size_tile_base<Size>::numThreads;
    _CG_STATIC_CONST_DECL unsigned int fullMask = 0xFFFFFFFF;

    _CG_STATIC_QUALIFIER unsigned int build_mask() {
        unsigned int mask = fullMask;
        if (numThreads != 32) {
            // [0,31] representing the current active thread in the warp
            unsigned int laneId = details::laneid();
            // shift mask according to the partition it belongs to
            mask = th::tileMask << (laneId & ~(th::laneMask));
        }
        return (mask);
    }

    _CG_QUALIFIER int any(int predicate) const {
        unsigned int lane_ballot = __ballot_sync(build_mask(), predicate);
        return (lane_ballot != 0);
    }
```

如果使用的是tile_helpers<32>的话:
- laneId 0的mask是, 0xFFFFFFFF,
- 对于laneId 1的mask是 0xFFFFFFFF << (1 & 0xFFFFFFE0) = 0xFFFFFFFF
- 对于laneId 2的mask是 0xFFFFFFFF << 0 = 0xFFFFFFFF
- 对于laneId 3的mask是 0xFFFFFFFF << 0 = 0xFFFFFFFF
- ...
- 对于laneId 15的mask是 0xFFFFFFFF << 0 = 0xFFFFFFFF

- 对于laneId 16的mask是 0xFFFFFFFF << 0 = 0xFFFFFFFF
- ...
- 对于laneId 31的mask是 0xFFFFFFFF << 0 = 0xFFFFFFFF

如果使用的是tile_helpers<16>的话:
- laneId 0的mask是, 0x0000FFFF,
- 对于laneId 1的mask是 0x0000FFFF << (1 & 0xFFFFFFF0) = (0XFFFF << 0) = 0XFFFF
- 对于laneId 2的mask是 0XFFFF << 0 = 0XFFFF
- 对于laneId 3的mask是 0XFFFF << 0 = 0XFFFF
- ...
- 对于laneId 15的mask是 0XFFFF << 0 = 0XFFFF

- 对于laneId 16的mask是 0XFFFF << 16 = 0XFFFF0000
- ...
- 对于laneId 31的mask是 0XFFFF << 1 = 0XFFFF0000

如果使用的是tile_helpers<4>的话:
- laneId 0的mask是, 0XF,
- 对于laneId 1的mask是 0XF << (1 & 0xFFFFFFFC) = (0XF << 0) = 0XF
- 对于laneId 2的mask是 0XF << 0 = 0XF
- 对于laneId 3的mask是 0XF << 0 = 0XF
- 对于laneId 4的mask是 0XF << (0x4 & 0xFFFFFFFC) = (0XF << 4) = 0X3C
- 对于laneId 5的mask是 0XF << 4 = 0X3C
- 对于laneId 6的mask是 0XF << 4 = 0X3C
- 对于laneId 7的mask是 0XF << 4 = 0X3C
- ...
- 对于laneId 28的mask是 0XF << 28 = 0XF0000000
- 对于laneId 29的mask是 0XF << 28 = 0XF0000000
- 对于laneId 30的mask是 0XF << 28 = 0XF0000000
- 对于laneId 31的mask是 0XF << 28 = 0XF0000000

如果使用的是tile_helpers<1>的话:
- laneId 0的mask是, 0x00000001,
- 对于laneId 1的mask是 0x00000001 << (1 & 0xFFFFFFFF) = (0X1 << 1) = 0X2
- 对于laneId 2的mask是 0X1 << 2 = 0X4
- 对于laneId 3的mask是 0X1 << 3 = 0X8
- ...
- 对于laneId 15的mask是 0X1 << 15 = 0X8000

- 对于laneId 16的mask是 0X1 << 16 = 0X10000
- ...
- 对于laneId 31的mask是 0x1 << 31 = 0X80000000

那么这个mask究竟代表了什么呢？

在CUDA programming guide中有如下一段话解释了mask：

___
For __all_sync, __any_sync, and __ballot_sync, a mask must be passed that specifies the threads participating in the call. A bit, representing the thread’s lane ID, must be set for each participating thread to ensure they are properly converged before the intrinsic is executed by the hardware.

Each calling thread must have its own bit set in the mask and all non-exited threads named in mask must execute the same intrinsic with the same mask, or the result is undefined.
___

这也就说明了如果是使用了一个tile_helpers<32>初始化的cooperative group，它们的mask都是0XFFFFFFFF，
但是对于一个tile_helpers<4>初始化的，它的mask是按照0XF,隔4 bit shift形成的.

nvidia在exec instruction的时候，hardware能够通过default PC reg，做indepent thread scheduling，exec mask应该是能够按照tree的方式，去collect，并且在执行指令之前判断是哪个sub warp level的执行.
