// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : madorsky
// File name : raw_delay.v
// Timestamp : Wed Jan 26 16:45:45 2022

module raw_delay
(
    din,
    dout,
    delay,
    we,
    trig_stop,
    clk
);

    input [383:0] din;
    output [383:0] dout;
    input [7:0] delay;
    input we;
    input trig_stop;
    input clk;

    reg [383:0] mem [255:0];
    // synthesis attribute ram_style of mem is block
    reg [7:0] adw;
    reg [7:0] adr;
    reg [7:0] adrr;
    always @(posedge clk) 
    begin
        if (trig_stop) 
        begin
            adw = 0;
            adr = (adw - delay) + 1;
        end
        else 
        begin
            if (we) mem[adw] = din;
            adrr = adr;
            adr = (adw - delay) + 1;
            adw = adw + 1;
        end
    end
    assign dout = mem[adrr];
endmodule
