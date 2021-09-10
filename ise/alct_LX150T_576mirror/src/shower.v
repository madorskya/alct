// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : ise
// File name : shower.v
// Timestamp : Fri Sep 10 17:48:44 2021

module shower
(
    ly0,
    ly1,
    ly2,
    ly3,
    ly4,
    ly5,
    th_loose,
    th_nominal,
    th_tight,
    shower_int,
    shower_oot,
    clk
);

    input [95:0] ly0;
    input [95:0] ly1;
    input [95:0] ly2;
    input [95:0] ly3;
    input [95:0] ly4;
    input [95:0] ly5;
    input [9:0] th_loose;
    input [9:0] th_nominal;
    input [9:0] th_tight;
    output [1:0] shower_int;
    reg    [1:0] shower_int;
    output [1:0] shower_oot;
    reg    [1:0] shower_oot;
    input clk;

    reg [6:0] i;
    reg [9:0] count;
    reg [9:0] slc [7:0];
    reg [7:0] loose;
    reg [7:0] nominal;
    reg [7:0] tight;
    always @(posedge clk) 
    begin
        tight = 0;
        nominal = 0;
        loose = 0;
        for (i = 0; i < 7; i = i + 1) 
        begin
            if (slc[i] >= th_tight) tight[i] = 1;
            else if (slc[i] >= th_nominal) nominal[i] = 1;
            else if (slc[i] >= th_loose) loose[i] = 1;
        end
        shower_int = 0;
        shower_oot = 0;
        if (tight[6:4] != 0) shower_int = 3;
        else if (nominal[6:4] != 0) shower_int = 2;
        else if (loose[6:4] != 0) shower_int = 1;
        if (tight[3:0] != 0) shower_oot = 3;
        else if (nominal[3:0] != 0) shower_oot = 2;
        else if (loose[3:0] != 0) shower_oot = 1;
        for (i = 0; i < 7; i = i + 1) slc[i] = slc[i + 1];
        slc[7] = count;
        count = 0;
        for (i = 0; i < 96; i = i + 1) count = count + ly0[i];
        for (i = 0; i < 96; i = i + 1) count = count + ly1[i];
        for (i = 0; i < 96; i = i + 1) count = count + ly2[i];
        for (i = 0; i < 96; i = i + 1) count = count + ly3[i];
        for (i = 0; i < 96; i = i + 1) count = count + ly4[i];
        for (i = 0; i < 96; i = i + 1) count = count + ly5[i];
    end
endmodule
