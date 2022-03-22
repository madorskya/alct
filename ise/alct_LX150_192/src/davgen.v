// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : madorsky
// File name : davgen.v
// Timestamp : Tue Mar 22 18:40:29 2022

module davgen
(
    l1a,
    davalid,
    clk
);

    input l1a;
    output davalid;
    reg    davalid;
    input clk;

    reg [10:0] l1a_sh;
    always @(posedge clk) 
    begin
        davalid = l1a_sh[0];
        l1a_sh = {l1a, l1a_sh[10:1]};
    end
endmodule
