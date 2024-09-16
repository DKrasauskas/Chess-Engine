

uint64_t random_uint64() {
    uint64_t u1, u2, u3, u4;
    u1 = (uint64_t)(rand()) & 0xFFFF; u2 = (uint64_t)(rand()) & 0xFFFF;
    u3 = (uint64_t)(rand()) & 0xFFFF; u4 = (uint64_t)(rand()) & 0xFFFF;
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}
unsigned int state = 1804289383;
unsigned int generate_random_number()
{
    // XOR shift algorithm
    unsigned int x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
    return x;
}
uint64_t Gen_Random_magic() {
    uint64_t u1, u2, u3, u4;

    // randomize numbers
    u1 = (uint64_t)(generate_random_number()) & 0xFFFF;
    u2 = (uint64_t)(generate_random_number()) & 0xFFFF;
    u3 = (uint64_t)(generate_random_number()) & 0xFFFF;
    u4 = (uint64_t)(generate_random_number()) & 0xFFFF;

    // shuffle bits and return
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}
uint64_t random_fewbits() {
    return  Gen_Random_magic() & Gen_Random_magic() & Gen_Random_magic();
}
int count_bits(uint64_t bitboard) {
    int count = 0;
    while (bitboard)
    {
        count++;
        bitboard &= bitboard - 1;
    }
    // return bit count
    return count;
}
