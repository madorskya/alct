
#ifndef _ALCTVER_H_
#define _ALCTVER_H_

#define LYN 6 // number of layers in the chamber

//#define ALCT384 1 // one of the ALCT288, ALCT384 or ALCT672 must be defined

#define PATTERNBITS 28 // how many bits in the collision pattern
#define WGBLK 8 // how many WGs in one block controlled by one set of pattern masks

#define LBITS 3 // number of bits in layers hit in pattern
#define QBITS 2 // number of bits for quality of hit output
#define ZEROQ "2'h0" // zero quality (related to QBITS, change them together)

#define KEYBITS 7 // key WG number bits
#define PATBBITS 1 // pattern B bits
#define AMBITS 1 // accelerator muon bits
#define DAQBITS 19 // daq readout bits

#define BESTBITS (KEYBITS + PATBBITS + AMBITS + QBITS)
#define IDSIZE 40 // size of the ID register
#define BWHC 32 // hit counter width

#ifdef ALCT672
#define HCBND {44, 80, 112} // segment boundaries for hit counters
#define HCNUM 3
#define WGN "3'h6"
#define BF "1'b1"
#define NP "1'b0"
#define ALCTTYPE "4'h6"
#define ALCTFB "4'h0"
#define ALCTPN "4'h0"
#define LYWG 112
#define lctinput Signal lct0_, Signal lct1_, Signal lct2_, Signal lct3_, Signal lct4_, Signal lct5_, Signal lct6_
#define lctin lct0_, lct1_, lct2_, lct3_, lct4_, lct5_, lct6_
#define lctptr   &lct0_, &lct1_, &lct2_, &lct3_, &lct4_, &lct5_, &lct6_
#define allbestn bestn[0] or bestn[1] or bestn[2] or bestn[3] or bestn[4] or bestn[5] or bestn[6]
#define BEST1        16				// number of inputs of the first stage of the best muon finder
#define BEST1BITS    4				// linked to the previous one
#define BEST2BITS    3			    // linked to BEST2
#ifdef MIRROR
#define ALCTNAME "alct672mirror"
#define ALCTPAR "4'b1"
#else
#define ALCTNAME "alct672"
#define ALCTPAR "4'b0"
#endif
#define MUX_DIR (!clk)
#define OSSIZE  ((AMBITS + QBITS + KEYBITS + PATBBITS) * 2 + DAQBITS + 9)//50  // output storage data size
#define OSINPUT (hfa, h, hn, validh, lfa, l, ln, validl, daqo, bxn(8,0)) 
#define OSINPVIND 39 // validh index in OS input
#define LCTCHIPS "19'b00000_1111111_1111111"
#define DLYLNGRP 7 // number of Delay line groups
#define PRSIZE  (2+7)   // Parameter register size

#endif
#ifdef ALCT576
#define HCBND {32, 64, 96} // segment boundaries for hit counters
#define HCNUM 3
#define WGN "3'h5"
#define BF "1'b1"
#define NP "1'b0"
#define ALCTTYPE "4'h5"
#define ALCTFB "4'h0"
#define ALCTPN "4'h0"
#define LYWG 96
#define lctinput Signal lct0_, Signal lct1_, Signal lct2_, Signal lct3_, Signal lct4_, Signal lct5_
#define lctin lct0_, lct1_, lct2_, lct3_, lct4_, lct5_
#define lctptr   &lct0_, &lct1_, &lct2_, &lct3_, &lct4_, &lct5_
#ifdef MIRROR
#define ALCTNAME "alct576mirror"
#define ALCTPAR "4'b1"
#else
#define ALCTNAME "alct576"
#define ALCTPAR "4'b0"
#endif
#define MUX_DIR (!clk)
#define OSSIZE  ((AMBITS + QBITS + KEYBITS + PATBBITS) * 2 + DAQBITS + 9)//50  // output storage data size
#define OSINPUT (hfa, h, hn, validh, lfa, l, ln, validl, daqo, bxn(8,0)) 
#define OSINPVIND 39 // validh index in OS input
#define LCTCHIPS "19'b00000_0111111_0111111"
#define DLYLNGRP 7 // number of Delay line groups
#define PRSIZE  (2+7)   // Parameter register size

#endif

#ifdef ALCT384
#define HCBND {16, 24, 28, 40, 48, 52, 64} // segment boundaries for hit counters
#define HCNUM 7
#define WGN "3'h3"
#define BF "1'b1"
#define NP "1'b0"
#define ALCTTYPE "4'h3"
#define ALCTFB "4'h0"
#define ALCTPN "4'h0"
#define LYWG 64
#define lctinput Signal lct0_, Signal lct1_, Signal lct2_, Signal lct3_
#define lctin lct0_, lct1_, lct2_, lct3_
#define lctptr   &lct0_, &lct1_, &lct2_, &lct3_
#define allbestn bestn[0] or bestn[1] or bestn[2] or bestn[3] or bestn[4] or bestn[5] or bestn[6] or bestn[7]
#define BEST1        8				// number of inputs of the first stage of the best muon finder
#define BEST1BITS    3				// linked to the previous one
#define BEST2BITS    3			    // linked to BEST2
#ifdef MIRROR
#define ALCTNAME "alct384mirror"
#define ALCTPAR "4'b1"
#else
#define ALCTNAME "alct384"
#define ALCTPAR "4'b0"
#endif
#define MUX_DIR (clk)
#define OSSIZE  ((AMBITS + QBITS + KEYBITS-1 + PATBBITS) * 2 + DAQBITS + 9)//50  // output storage data size
#define OSINPUT (hfa, h, hn(5,0), validh, lfa, l, ln(5,0), validl, daqo, bxn(8,0)) 
#define OSINPVIND 38 // validh index in OS input
#define LCTCHIPS "19'b00000_0001111_0001111"
#define DLYLNGRP 4 // number of Delay line groups
#define PRSIZE  (2+4)   // Parameter register size

#endif

#ifdef ALCT192
#define HCBND {12, 22, 32} // segment boundaries for hit counters
#define HCNUM 3
#define WGN "3'h1"
#define BF "1'b1"
#define NP "1'b0"
#define ALCTTYPE "4'h1"
#define ALCTFB "4'h0"
#define ALCTPN "4'h0"
#define LYWG 32
#define lctinput Signal lct0_, Signal lct1_
#define lctin lct0_, lct1_
#define lctptr   &lct0_, &lct1_
#ifdef MIRROR
#define ALCTNAME "alct192mirror"
#define ALCTPAR "4'b1"
#else
#define ALCTNAME "alct192"
#define ALCTPAR "4'b0"
#endif
#define MUX_DIR (clk)
#define OSSIZE  ((AMBITS + QBITS + KEYBITS-1 + PATBBITS) * 2 + DAQBITS + 9)//50  // output storage data size
#define OSINPUT (hfa, h, hn(5,0), validh, lfa, l, ln(5,0), validl, daqo, bxn(8,0)) 
#define OSINPVIND 38 // validh index in OS input
#define LCTCHIPS "19'b00000_0000011_0000011"
#define DLYLNGRP 3 // number of Delay line groups
#define PRSIZE  (2+3)   // Parameter register size

#endif


#ifdef ALCT288
#define HCBND {48} // segment boundaries for hit counters
#define HCNUM 1
#define WGN "3'h2"
#define ALCTTYPE "4'h2"
#define LYWG 48
#define lctinput Signal lct0_, Signal lct1_, Signal lct2_
#define lctin lct0_, lct1_, lct2_
#define lctptr   &lct0_, &lct1_, &lct2_
#define allbestn bestn[0] or bestn[1] or bestn[2] or bestn[3] or bestn[4] or bestn[5]
#define BEST1        8				// number of inputs of the first stage of the best muon finder
#define BEST1BITS    3				// linked to the previous one
#define BEST2BITS    3			    // linked to BEST2
#define DLYLNGRP (LYWG/16) // number of Delay line groups
#define PRSIZE  (2+3)   // Parameter register size

#ifdef ME11FN
#define BF "1'b1"
#define NP "1'b0"
#define ALCTNAME "alct288fn"
#define ALCTFB "4'h2"
#define ALCTPN "4'h0"
#define ALCTPAR "4'h0"
#else
#ifdef ME11FP
#define BF "1'b1"
#define NP "1'b1"
#define ALCTNAME "alct288fp"
#define ALCTFB "4'h2"
#define ALCTPN "4'h4"
#define ALCTPAR "4'h0"
#else
#ifdef ME11BN
#define BF "1'b0"
#define NP "1'b0"
#define ALCTNAME "alct288bn"
#define ALCTFB "4'h0"
#define ALCTPN "4'h0"
#define ALCTPAR "4'h0"
#else
#ifdef ME11BP
#define BF "1'b0"
#define NP "1'b1"
#define ALCTNAME "alct288bp"
#define ALCTFB "4'h0"
#define ALCTPN "4'h4"
#define ALCTPAR "4'h0"
#else
#define BF "1'b1"
#define NP "1'b0"
#define ALCTFB "4'h2"
#define ALCTPN "4'h0"
#ifdef MIRROR
#define ALCTNAME "alct288mirror"
#define ALCTPAR "4'h1"
#else
#define ALCTNAME "alct288"
#define ALCTPAR "4'h0"
#endif
#endif
#endif
#endif
#endif

#define MUX_DIR (clk)
#define OSSIZE  ((AMBITS + QBITS + KEYBITS-1 + PATBBITS) * 2 + DAQBITS + 9)//50  // output storage data size
#define OSINPUT (hfa, h, hn(5,0), validh, lfa, l, ln(5,0), validl, daqo, bxn(8,0)) 
#define OSINPVIND 38 // validh index in OS input
#define LCTCHIPS "19'b00000_0000111_0000111"

#endif

#define COLLMASKBITS (PATTERNBITS * LYWG / WGBLK) // how many bits are in collision mask register
#define HCMASKBITS   (LYWG * LYN) // how many bits are in hot channel mask
#define HITSBITS     (LYWG * QBITS) // how many bits are in the hits register
#define BEST2        (LYWG / BEST1) // number of inputs of the second stage of the best muon finder
#define ExTime       5 // one shot extension time

#define CRSIZE  69  // Configuration register size
#define YRSIZE  31  // output register size

#define LCTS     (LYWG/16) // number of input buses

#define DAQCRC 1 // if this is defined, DAQ information will contain CRC

#define MXBXN 12 // BX counter bit width

#define tvalid(a) (a(3))

#ifdef MIRROR
#define MR "1'b1"
#else
#define MR "1'b0"
#endif

#ifdef KILLEQUAL
#define KE "1'b1"
#else
#define KE "1'b0"
#endif

#ifdef RED_LAT
#define RL "1'b1"
#define PB "1'b0"
#else
#define RL "1'b0"
#define PB "1'b1"
#endif

#ifdef S6
#define SP6 "1'b1"
#else
#define SP6 "1'b0"
#endif


// for asynch alct
#define BWTMARK 12 // 12 bit bxn 

#endif
