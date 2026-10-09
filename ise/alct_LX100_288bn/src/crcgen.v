// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : mador
// File name : crcgen.v
// Timestamp : Fri Oct  9 16:17:28 2026

module crcgen
(
    d,
    crc,
    calc,
    clk
);

    input [27:0] d;
    output [27:0] crc;
    reg    [27:0] crc;
    input [2:0] calc;
    input clk;

    reg [21:0] ncrc;
    reg [5:0] i;
    reg t;
    // This module implements CRC generation algorithm
    // with the following parameters:
    // Poly           = 10000000000000000000011
    // Data width     = 28
    // CRC width      = 22
    // CRC init       = 0
    // Data bit first = MSB
    always @(posedge clk) 
    begin
        crc = d;
        case (calc)
            0 : 
            begin
                ncrc = 0;
            end
            1 : 
            begin
                for (i = 28; i > 0; i = i - 1) 
                begin
                    t = d[i - 1] ^ ncrc[21];
                    ncrc[21:2] = ncrc[20:1];
                    ncrc[1] = t ^ ncrc[0];
                    ncrc[0] = t;
                end
            end
            2 : 
            begin
                crc = {3'b010, 3'b000, ncrc};
            end
        endcase
    end
endmodule
