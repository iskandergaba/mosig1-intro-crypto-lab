# Cryptography Lab

## Part One
### Question 1
TODO

### Question 2
Done. Check the code.

### Question 3
Done. Check the code.

## Part Two
### Question 1
- **The attack:** Explore the fact that after encrypting a certain number of blocks with the same block cipher key in CBC mode, collision is very likely to happen. If ci and cj are two identical cipher text blocks, then c(i - 1) XOR c(j - 1) = m(i - 1) XOR m(j - 1).
- **Data complexity:** Collision occurs with a high probability after observing 2^(n/2) cipher text blocks encrypted with the same key. Therefore, the data complexity is approximately 2^(n/2) * n.

### Question 2
- To handle and find colliding cipher text blocks efficiently, one can use a hash table data structure where the keys of entries are the cipher text blocks and values are the indices.
- For SPECK 32/64, on average, the amount of data needed for a collision to be almost certainly found is between 2^17 and 2^18 cipher text blocks. This is consistent with the theoretical expectations of this attack as it proved to be very hard to find a collision for a cipher text consisting of less than 2^16 cipher text blocks.

### Question 3
- For SPECK 48/96, on average, the amount of data needed for a collision to be almost certainly found is between 2^25 and 2^26 cipher text blocks. This is consistent with the theoretical expectations of this attack as it proved to be very hard to find a collision for a cipher text consisting of less than 2^24 cipher text blocks.

### Question 4
The method is implemented, but not tested.

### Question 5
- No it would not be feasible on a decent desktop for this will require a crazy amount of at least 11 Terabyte of memory to do so, i.e. (80 * 2^40) bits. With that being said, it is obviously not possible to succeed with this attack on a 128-bit long block cipher using a decent desktop.

### Question 6
TODO