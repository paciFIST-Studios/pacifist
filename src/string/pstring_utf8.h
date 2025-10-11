// paciFIST studios. 2025. MIT License

#ifndef PACIFIST_PSTRING_UTF8_H
#define PACIFIST_PSTRING_UTF8_H

/* Notes on UTF-8 decoding:
 *
 *     Starts With:                Is Type:
 *      0-------                    1-byte utf8
 *      110-----                    2-byte utf8
 *      1110----                    3-byte utf8 
 *      11110---                    4-byte utf8 
 * 
 *      10------                    follow-on-byte 
 * 
 *     Example:
 *             ccc   cccccc   cccccc   cccccc
 *             ...   ......   ......   ......
 *        11110000 10011111 10001101 10001110
 *        ^^^^^    ^^       ^^       ^^
 *        a        b        b        b
 *
 *        a: indicates a 4-byte utf8 character
 *        b: indicates a proper follow-on character
 *        c: data bits: 21 total
 *        
 *        d: data payload: 000011111001101001110
 *        e: nibbles: 0 0001 1111 0011 0100 1110
 *        f: hex:     0 1    F    3    4    E
 *        g: unicode: U+01F34E
 *        h: emoji: 🍎
 *
 *
 *    Example2:
 *
 *          bbbbbbb
 *          .......
 *         00101011
 *         ^
 *         a
 *         
 *         a: indicates a 1-byte utf8 character
 *         b: data bits: 7 total
 *         
 *         c: data payload: 0101011
 *         d: nibbles: 010 1011
 *         e: hex:     2   B
 *         f: unicode: U+002B
 *         h: character: +
 *
 *
 *    Example3:
 *
 *           ccccc   cccccc
 *           .....   ......
 *        11001111 10000000
 *        ^^^      ^^
 *        a        b
 *
 *        a: indicates a 2-byte utf-8 character
 *        b: indicates a follow-on byte
 *        c: data bits: 11 total
 *
 *        d: data payload: 01111000000
 *        e: nibbles:      011 1100 0000
 *        f: hex           3   C    0
 *        g: unicode: U+03C0
 *        h: character: π
 *
 *    Example4:
 *
 *         bbbbbbb
 *         .......
 *        00100001
 *        ^
 *        a
 *
 *        a: indicates a 1-byte utf-8 character
 *        b: data bits: 7 total
 *
 *        c: data payload: 0100001
 *        d: nibbles:      010 0001
 *        e: hex:          2   1
 *        f: unicode: U+0021
 *        g: character: ! 
 *
 *
 *
 *     Converted message:
 *
 *     11110000 10011111 10001101 10001110
 *     00101011 11001111 10000000 00100001
 *
 *     🍎+π!
 *
 *
 *
 *
 */







#endif //PACIFIST_PSTRING_UTF8_H