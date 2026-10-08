// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : mador
// File name : daq_gbtx_26.v
// Timestamp : Thu Oct  8 16:42:59 2026

module daq_gbtx_26
(
    ly0,
    ly1,
    ly2,
    ly3,
    ly4,
    ly5,
    best1,
    best2,
    shower_int,
    bxn,
    fifo_tbins,
    daqp,
    daq_gbtx_valid,
    l1a_delay,
    fifo_pretrig,
    fifo_mode,
    L1A,
    hard_rst,
    l1a_internal,
    l1a_window,
    l1a_offset,
    L1AWindow,
    l1aTP,
    validhd,
    send_empty,
    config_report_i,
    bxn_before_reset,
    virtex_id,
    trig_reg,
    config_reg,
    hot_channel_mask,
    collision_mask,
    zero_suppress,
    trig_stop,
    seu_error,
    clk,
    clk160
);

    input [47:0] ly0;
    input [47:0] ly1;
    input [47:0] ly2;
    input [47:0] ly3;
    input [47:0] ly4;
    input [47:0] ly5;
    input [10:0] best1;
    input [10:0] best2;
    input [1:0] shower_int;
    input [11:0] bxn;
    input [4:0] fifo_tbins;
    output [27:0] daqp;
    output daq_gbtx_valid;
    reg    daq_gbtx_valid;
    input [7:0] l1a_delay;
    input [4:0] fifo_pretrig;
    input [1:0] fifo_mode;
    input L1A;
    input hard_rst;
    input l1a_internal;
    input [3:0] l1a_window;
    input [3:0] l1a_offset;
    output L1AWindow;
    output l1aTP;
    output validhd;
    input send_empty;
    input config_report_i;
    input [11:0] bxn_before_reset;
    input [39:0] virtex_id;
    input [2:0] trig_reg;
    input [68:0] config_reg;
    input [287:0] hot_channel_mask;
    input [167:0] collision_mask;
    input zero_suppress;
    input trig_stop;
    input seu_error;
    input clk;
    input clk160;

    wire [10:0] best1e;
    wire [10:0] best2e;
    wire [11:0] bxne;
    wire [1:0] shower_e;
    reg [10:0] best1d;
    reg [10:0] best2d;
    reg [11:0] bxnd;
    reg [1:0] shower_d;
    wire [47:0] lyd0;
    wire [47:0] lyd1;
    wire [47:0] lyd2;
    wire [47:0] lyd3;
    wire [47:0] lyd4;
    wire [47:0] lyd5;
    wire [5:0] lyzd;
    wire [7:0] l1a_int_delay;
    wire l1a_proc;
    wire best_we;
    wire raw_we;
    wire valor;
    reg l1a_procr;
    reg [11:0] bxnr;
    reg [7:0] best_adw;
    reg [7:0] raw_adw;
    wire best_full;
    wire raw_full;
    wire [11:0] l1a_in_count;
    reg [6:0] rst_cnt;
    wire l1a_procd;
    wire [11:0] l1a_in_countd;
    wire best_fulld;
    wire raw_fulld;
    wire [7:0] best_adwd;
    wire [7:0] raw_adwd;
    wire [11:0] bxnrd;
    wire l1a_fifo_we;
    wire l1a_fifo_full;
    wire l1a_bxn_fifo_full;
    reg l1a_fifo_re;
    wire l1a_fifo_empty;
    wire l1a_bxn_fifo_empty;
    wire [11:0] l1a_in_countf;
    wire best_fullf;
    wire raw_fullf;
    wire [7:0] best_adwf;
    wire [7:0] raw_adwf;
    wire [11:0] bxn_l1a;
    wire [10:0] best1m;
    wire [10:0] best2m;
    wire [11:0] bxnm;
    wire [1:0] shower_m;
    wire [47:0] lym0;
    wire [47:0] lym1;
    wire [47:0] lym2;
    wire [47:0] lym3;
    wire [47:0] lym4;
    wire [47:0] lym5;
    wire [5:0] lyzm;
    reg [7:0] best_adr;
    reg [7:0] best_adb;
    reg [7:0] raw_adr;
    reg [7:0] raw_adb;
    reg [11:0] readout_count;
    reg [4:0] state;
    reg [2:0] idle_cnt;
    reg [1:0] wphase;
    reg [3:0] best_cnt;
    reg [3:0] lct_bins;
    reg [3:0] lct_bins_report;
    reg [4:0] raw_bins;
    reg [47:0] lyt [5:0];
    reg [5:0] lzt;
    reg [5:0] raw_cnt;
    reg [2:0] ly_cnt;
    reg [3:0] wg_cnt;
    reg ly_zero;
    reg [2:0] crccalc;
    reg [27:0] daqw;
    reg daqv_w;
    reg [10:0] frame_count;
    wire [5:0] fwver;
    assign fwver = 6'h7;
    assign l1a_int_delay = l1a_delay - 1;
    assign L1AWindow = best_we;
    assign l1aTP = l1a_proc;
    assign validhd = best1d[3];
    best_delay best_delay
    (
        {shower_int, best1, best2, bxn},
        {shower_e, best1e, best2e, bxne},
        l1a_int_delay,
        hard_rst,
        l1a_procr,
        l1a_window,
        best1[3] || (shower_int > 1),
        valor,
        trig_stop,
        clk
    );
    raw_delay raw_delay
    (
        {ly0, ly1, ly2, ly3, ly4, ly5},
        {lyd0, lyd1, lyd2, lyd3, lyd4, lyd5},
        l1a_delay + fifo_pretrig,
        hard_rst,
        trig_stop,
        clk
    );
    assign lyzd = {lyd5 == 0, lyd4 == 0, lyd3 == 0, lyd2 == 0, lyd1 == 0, lyd0 == 0};
    l1a_maker_ l1a_maker_
    (
        L1A,
        valor,
        best1e[3] || (shower_e > 1),
        l1a_proc,
        l1a_window,
        fifo_tbins,
        l1a_fifo_full,
        best_full,
        raw_full,
        fifo_mode != 0,
        best_we,
        raw_we,
        l1a_internal,
        l1a_in_count,
        l1a_offset,
        send_empty,
        hard_rst,
        clk
    );
    l1a_dly l1a_dly
    (
        {l1a_procr, l1a_in_count, best_full, raw_full, best_adw, raw_adw, bxnr},
        {l1a_procd, l1a_in_countd, best_fulld, raw_fulld, best_adwd, raw_adwd, bxnrd},
        6'd34,
        1'b1,
        1'b0,
        clk
    );
    assign l1a_fifo_we = l1a_procd && rst_cnt[6];
    always @(posedge clk) 
    begin
        if (!hard_rst) 
        begin
            best_adw = 1;
            raw_adw = 1;
            l1a_procr = 0;
            rst_cnt = 0;
        end
        else 
        begin
            if (best_we) best_adw = best_adw + 1;
            if (raw_we) raw_adw = raw_adw + 1;
            if (!rst_cnt[6]) rst_cnt = rst_cnt + 1;
            l1a_procr = l1a_proc;
            {shower_d, best1d, best2d, bxnd} = {shower_e, best1e, best2e, bxne};
            bxnr = bxn;
        end
    end
    l1a_fifo l1a_fifo
    (
        {l1a_in_countd, best_fulld, raw_fulld, best_adwd, raw_adwd},
        {l1a_in_countf, best_fullf, raw_fullf, best_adwf, raw_adwf},
        l1a_fifo_we,
        l1a_fifo_re,
        !hard_rst,
        l1a_fifo_empty,
        l1a_fifo_full,
        clk,
        clk160
    );
    l1a_bxn_fifo l1a_bxn_fifo
    (
        bxnrd,
        bxn_l1a,
        l1a_fifo_we,
        l1a_fifo_re,
        !hard_rst,
        l1a_bxn_fifo_empty,
        l1a_bxn_fifo_full,
        clk,
        clk160
    );
    best_memory best_memory
    (
        best_adw,
        best_adr,
        best_adb,
        {shower_d, best1d, best2d, bxnd},
        {shower_m, best1m, best2m, bxnm},
        best_we,
        {4'b0, l1a_window},
        best_full,
        clk,
        clk160
    );
    raw_memory raw_memory
    (
        raw_adw,
        raw_adr,
        raw_adb,
        {lyzd, lyd0, lyd1, lyd2, lyd3, lyd4, lyd5},
        {lyzm, lym0, lym1, lym2, lym3, lym4, lym5},
        raw_we,
        {3'b0, fifo_tbins},
        raw_full,
        clk,
        clk160
    );
    crcgen crcgen
    (
        daqw,
        daqp,
        crccalc,
        clk160
    );
    always @(posedge clk160) 
    begin
        wphase = wphase + 1;
        daq_gbtx_valid = daqv_w;
        if (!hard_rst) 
        begin
            readout_count = l1a_offset;
            state = 0;
            best_adb = 0;
            raw_adb = 0;
            crccalc = 0;
            idle_cnt = 0;
            daqw = 28'h051E57A;
            daqv_w = 0;
        end
        else 
        begin
            daqv_w = (state >= 5) && (state <= 15);
            daqw = 28'h051E57A;
            l1a_fifo_re = 0;
            case (state)
                0 : 
                begin
                    crccalc = 0;
                    if (((!l1a_fifo_empty) && (idle_cnt == 4)) && (wphase == 3)) 
                    begin
                        state = 1;
                        l1a_fifo_re = 1;
                    end
                    if (idle_cnt != 4) idle_cnt = idle_cnt + 1;
                end
                1 : 
                begin
                    state = 2;
                end
                2 : 
                begin
                    raw_adr = raw_adwf;
                    raw_adb = raw_adwf;
                    state = 3;
                end
                3 : 
                begin
                    state = 4;
                end
                4 : 
                begin
                    best_adr = best_adwf;
                    best_adb = best_adwf;
                    best_cnt = 0;
                    frame_count = 0;
                    lct_bins = (best_fullf) ? 0 : l1a_window;
                    raw_bins = ((fifo_mode != 0) && (!raw_fullf)) ? fifo_tbins : 0;
                    lct_bins_report = (lct_bins != 0) ? lct_bins + 4'b1 : 4'b0;
                    state = 5;
                end
                5 : 
                begin
                    daqw = 28'h5E750FF;
                    state = 6;
                    crccalc = 1;
                end
                6 : 
                begin
                    daqw = {2'b01, bxn_l1a, 2'b00, l1a_in_countf};
                    state = 7;
                end
                7 : 
                begin
                    daqw = {2'b01, readout_count, 2'b00, bxnm};
                    state = 8;
                end
                8 : 
                begin
                    daqw = {2'b01, bxn_before_reset, 2'b00, 3'b000, lct_bins_report, raw_bins};
                    best_adr = best_adr + 1;
                    state = 9;
                end
                9 : 
                begin
                    daqw = {1'b0, trig_reg, 1'b0, 2'b00, 1'b1, 1'b0, 1'b1, 1'b0, 3'h2, 2'b00, fwver, 1'b1, 1'b0, seu_error, best_fullf, raw_fullf, zero_suppress};
                    best_adr = best_adr + 1;
                    raw_cnt = 0;
                    ly_cnt = 0;
                    wg_cnt = 0;
                    if (lct_bins != 0) state = 10;
                    else 
                    begin
                        lyt[0] = lym0;
                        lyt[1] = lym1;
                        lyt[2] = lym2;
                        lyt[3] = lym3;
                        lyt[4] = lym4;
                        lyt[5] = lym5;
                        lzt = lyzm;
                        raw_adr = raw_adr + 1;
                        state = (raw_bins != 0) ? 11 : (frame_count[1:0] == 1) ? 13 : 12;
                    end
                end
                10 : 
                begin
                    if (best_cnt == lct_bins) daqw = 28'h0;
                    else daqw = {2'b00, best2m[10:4], 1'b0, best2m[2:0], best2m[3], shower_m, best1m[10:4], 1'b0, best1m[2:0], best1m[3]};
                    best_adr = best_adr + 1;
                    if (best_cnt == lct_bins) 
                    begin
                        lyt[0] = lym0;
                        lyt[1] = lym1;
                        lyt[2] = lym2;
                        lyt[3] = lym3;
                        lyt[4] = lym4;
                        lyt[5] = lym5;
                        lzt = lyzm;
                        raw_adr = raw_adr + 1;
                        state = (raw_bins != 0) ? 11 : (frame_count[1:0] == 1) ? 13 : 12;
                    end
                    best_cnt = best_cnt + 1;
                end
                11 : 
                begin
                    ly_zero = zero_suppress && lzt[0];
                    if (ly_zero) daqw = 28'h4000000;
                    else daqw = {2'b00, lyt[0][23:12], 2'b00, lyt[0][11:0]};
                    if (ly_zero || (wg_cnt == 1)) 
                    begin
                        wg_cnt = 0;
                        if (ly_cnt == 5) 
                        begin
                            ly_cnt = 0;
                            raw_cnt = raw_cnt + 1;
                            lyt[0] = lym0;
                            lyt[1] = lym1;
                            lyt[2] = lym2;
                            lyt[3] = lym3;
                            lyt[4] = lym4;
                            lyt[5] = lym5;
                            lzt = lyzm;
                            raw_adr = raw_adr + 1;
                            if (raw_cnt == raw_bins) state = (frame_count[1:0] == 1) ? 13 : 12;
                        end
                        else 
                        begin
                            ly_cnt = ly_cnt + 1;
                            lyt[0] = lyt[1];
                            lyt[1] = lyt[2];
                            lyt[2] = lyt[3];
                            lyt[3] = lyt[4];
                            lyt[4] = lyt[5];
                            lzt = {1'b0, lzt[5:1]};
                        end
                    end
                    else 
                    begin
                        wg_cnt = wg_cnt + 1;
                        lyt[0] = {24'd0, lyt[0][47:24]};
                    end
                end
                12 : 
                begin
                    daqw = 28'hC000000;
                    if (frame_count[1:0] == 1) state = 13;
                end
                13 : 
                begin
                    state = 14;
                    daqw = 28'hDECEA5E;
                    crccalc = 4;
                end
                14 : 
                begin
                    state = 15;
                    daqw = 28'h0;
                    crccalc = 2;
                end
                15 : 
                begin
                    state = 0;
                    daqw = {3'b010, 14'h0, frame_count};
                    readout_count = readout_count + 1;
                    crccalc = 0;
                    idle_cnt = 0;
                end
                default : state = 0;
            endcase
            frame_count = frame_count + 1;
        end
    end
endmodule
