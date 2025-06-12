#ifndef MEMORY_ALIGNER_H
#define MEMORY_ALIGNER_H

#define M_MMAP_THRESHOLD  128*1024

#define ALIGNER_BIT       16
#define ALIGNER_UNIVERSAL 15
#define ALIGNER_64BIT     63

#define ALIGNER_LARGEBIN  64

#define IS_MMAPPED_FLAG          0x2
#define IS_MMAPPED_DECODE_FLAG   0xF

#define ALIGN_SIZE(size) (((size) + ALIGNER_UNIVERSAL) & ~ALIGNER_UNIVERSAL)
#define ALIGN_SIZE_64BIT(size) (((size) + ALIGNER_64BIT) & ~ALIGNER_64BIT)

#define SMALLBIN_START_SIZE(header_size) ((NUM_FASTBINS * ALIGNER_BIT) + ALIGN_SIZE(header_size))

#define CALCULATE_FAST_BIN_INDEX(size, header_size) ((size - ALIGN_SIZE(header_size) + ALIGNER_UNIVERSAL) / ALIGNER_BIT)
#define CALCULATE_SMALL_BIN_INDEX(size, header_size) (((size) - SMALLBIN_START_SIZE(header_size)) / ALIGNER_BIT)

// increment the bin index every 256 bytes of block size after a certain threshold, starting at size 512
#define CALCULATE_LARGE_BIN_INDEX(size) ((size / (M_MMAP_THRESHOLD / ALIGNER_LARGEBIN)) + SMALLBIN_THRESHOLD)

#define ENCODE_AS_MMAPPED(size) (size | IS_MMAPPED_FLAG)
#define DECODE_MMAPPED_SIZE(size) (size & ~IS_MMAPPED_DECODE_FLAG)
#define IS_MMAPPED(size) (size & IS_MMAPPED_FLAG)

#endif