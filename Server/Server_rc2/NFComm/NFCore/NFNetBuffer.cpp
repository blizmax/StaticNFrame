#include "NFNetBuffer.h"

const char NFNetBuffer::kCRLF[] = "\r\n";

const size_t NFNetBuffer::kCheapPrependSize = 8;
const size_t NFNetBuffer::kInitialSize  = 1024;