// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : madorsky
// File name : raw_memory.v
// Timestamp : Tue Apr 19 16:44:52 2022

module raw_memory
(
    adw,
    adr,
    adb,
    dw,
    dr,
    we,
    wblock,
    full,
    clk
);

    input [7:0] adw;
    input [7:0] adr;
    input [7:0] adb;
    input [287:0] dw;
    output [287:0] dr;
    input we;
    input [7:0] wblock;
    output full;
    input clk;

    reg [7:0] adrr;
    reg [287:0] mem [255:0];
    // synthesis attribute ram_style of mem is block
    wire [7:0] diff;
    assign diff = adb - adw;
    assign full = !((diff > (wblock + 10)) || (adb == adw));
    always @(posedge clk) 
    begin
        if (we) mem[adw] = dw;
        adrr = adr;
    end
    assign dr = mem[adrr];
endmodule
