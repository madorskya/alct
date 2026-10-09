// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : mador
// File name : raw_memory.v
// Timestamp : Fri Oct  9 16:18:00 2026

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
    wclk,
    rclk
);

    input [7:0] adw;
    input [7:0] adr;
    input [7:0] adb;
    input [293:0] dw;
    output [293:0] dr;
    reg    [293:0] dr;
    input we;
    input [7:0] wblock;
    output full;
    input wclk;
    input rclk;

    reg [7:0] adrr;
    reg [293:0] mem [255:0];
    // synthesis attribute ram_style of mem is block
    reg [7:0] adb_r1;
    reg [7:0] adb_r2;
    reg [7:0] adb_s;
    // synthesis attribute ASYNC_REG of adb_r1 is TRUE
    // synthesis attribute ASYNC_REG of adb_r2 is TRUE
    wire [7:0] diff;
    assign diff = adb_s - adw;
    assign full = !((diff > (wblock + 10)) || (adb_s == adw));
    always @(posedge wclk) 
    begin
        if (we) mem[adw] = dw;
        if (adb_r1 == adb_r2) adb_s = adb_r2;
        adb_r2 = adb_r1;
        adb_r1 = adb;
    end
    always @(posedge rclk) 
    begin
        dr = mem[adrr];
        adrr = adr;
    end
endmodule
