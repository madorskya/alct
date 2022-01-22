// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : madorsky
// File name : shower.v
// Timestamp : Sat Jan 22 18:18:42 2022

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
    clk
);

    input [47:0] ly0;
    input [47:0] ly1;
    input [47:0] ly2;
    input [47:0] ly3;
    input [47:0] ly4;
    input [47:0] ly5;
    input [9:0] th_loose;
    input [9:0] th_nominal;
    input [9:0] th_tight;
    output [1:0] shower_int;
    reg    [1:0] shower_int;
    input clk;

    reg [6:0] i;
    reg [9:0] count;
    reg [2:0] ly_count;
    reg [12:0] slc [4:0];
    reg loose;
    reg nominal;
    reg tight;
    always @(posedge clk) 
    begin
        tight = 0;
        nominal = 0;
        loose = 0;
        if (slc[0][12:10] >= 3'd5) 
        begin
            if (slc[0][9:0] >= th_tight) tight = 1;
            else if (slc[0][9:0] >= th_nominal) nominal = 1;
            else if (slc[0][9:0] >= th_loose) loose = 1;
        end
        shower_int = 0;
        if (tight == 1) shower_int = 3;
        else if (nominal == 1) shower_int = 2;
        else if (loose == 1) shower_int = 1;
        for (i = 0; i < 4; i = i + 1) 
        begin
            slc[i] = slc[i + 1];
        end
        slc[4] = {ly_count, count};
        count = 0;
        ly_count = 0;
        for (i = 0; i < 48; i = i + 1) count = count + ly0[i];
        for (i = 0; i < 48; i = i + 1) count = count + ly1[i];
        for (i = 0; i < 48; i = i + 1) count = count + ly2[i];
        for (i = 0; i < 48; i = i + 1) count = count + ly3[i];
        for (i = 0; i < 48; i = i + 1) count = count + ly4[i];
        for (i = 0; i < 48; i = i + 1) count = count + ly5[i];
        if (ly0 != 0) ly_count = ly_count + 1;
        if (ly1 != 0) ly_count = ly_count + 1;
        if (ly2 != 0) ly_count = ly_count + 1;
        if (ly3 != 0) ly_count = ly_count + 1;
        if (ly4 != 0) ly_count = ly_count + 1;
        if (ly5 != 0) ly_count = ly_count + 1;
    end
endmodule
