// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : mador
// File name : l1a_bxn_fifo.v
// Timestamp : Fri Oct  9 16:18:35 2026

module l1a_bxn_fifo
(
    din,
    dout,
    wen,
    ren,
    reset,
    empty,
    full,
    wclk,
    rclk
);

    input [11:0] din;
    output [11:0] dout;
    input wen;
    input ren;
    input reset;
    output empty;
    reg    empty;
    output full;
    reg    full;
    input wclk;
    input rclk;

    reg [11:0] mem [255:0];
    // synthesis attribute ram_style of mem is distributed
    reg [7:0] waddr;
    reg [7:0] raddr;
    reg [7:0] raddrr;
    reg [7:0] wgray;
    reg [7:0] rgray;
    reg [7:0] wgray_r1;
    reg [7:0] wgray_r2;
    reg [7:0] rgray_r1;
    reg [7:0] rgray_r2;
    reg [7:0] waddr_n;
    // synthesis attribute ASYNC_REG of wgray_r1 is TRUE
    // synthesis attribute ASYNC_REG of wgray_r2 is TRUE
    // synthesis attribute ASYNC_REG of rgray_r1 is TRUE
    // synthesis attribute ASYNC_REG of rgray_r2 is TRUE
    always @(posedge wclk) 
    begin
        if (wen && (!full)) 
        begin
            mem[waddr] = din;
            waddr = waddr + 1;
        end
        if (reset) 
        begin
            waddr = 0;
        end
        wgray = waddr ^ {1'd0, waddr[7:1]};
        waddr_n = waddr + 1;
        rgray_r2 = rgray_r1;
        rgray_r1 = rgray;
        full = (waddr_n ^ {1'd0, waddr_n[7:1]}) == rgray_r2;
    end
    always @(posedge rclk) 
    begin
        if (ren && (!empty)) 
        begin
            raddrr = raddr;
            raddr = raddr + 1;
        end
        if (reset) 
        begin
            raddr = 0;
        end
        rgray = raddr ^ {1'd0, raddr[7:1]};
        wgray_r2 = wgray_r1;
        wgray_r1 = wgray;
        empty = wgray_r2 == rgray;
    end
    assign dout = mem[raddrr];
endmodule
