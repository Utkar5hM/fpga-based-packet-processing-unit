-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.1 (win64) Build 2552052 Fri May 24 14:49:42 MDT 2019
-- Date        : Mon Jan 22 11:49:08 2024
-- Host        : utg14-win running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               c:/Users/utkar/majorproject/majorproject.srcs/sources_1/bd/mbdesign/ip/mbdesign_mii_to_rmii_0_0/mbdesign_mii_to_rmii_0_0_stub.vhdl
-- Design      : mbdesign_mii_to_rmii_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7a100tcsg324-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity mbdesign_mii_to_rmii_0_0 is
  Port ( 
    rst_n : in STD_LOGIC;
    ref_clk : in STD_LOGIC;
    mac2rmii_tx_en : in STD_LOGIC;
    mac2rmii_txd : in STD_LOGIC_VECTOR ( 3 downto 0 );
    mac2rmii_tx_er : in STD_LOGIC;
    rmii2mac_tx_clk : out STD_LOGIC;
    rmii2mac_rx_clk : out STD_LOGIC;
    rmii2mac_col : out STD_LOGIC;
    rmii2mac_crs : out STD_LOGIC;
    rmii2mac_rx_dv : out STD_LOGIC;
    rmii2mac_rx_er : out STD_LOGIC;
    rmii2mac_rxd : out STD_LOGIC_VECTOR ( 3 downto 0 );
    phy2rmii_crs_dv : in STD_LOGIC;
    phy2rmii_rx_er : in STD_LOGIC;
    phy2rmii_rxd : in STD_LOGIC_VECTOR ( 1 downto 0 );
    rmii2phy_txd : out STD_LOGIC_VECTOR ( 1 downto 0 );
    rmii2phy_tx_en : out STD_LOGIC
  );

end mbdesign_mii_to_rmii_0_0;

architecture stub of mbdesign_mii_to_rmii_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "rst_n,ref_clk,mac2rmii_tx_en,mac2rmii_txd[3:0],mac2rmii_tx_er,rmii2mac_tx_clk,rmii2mac_rx_clk,rmii2mac_col,rmii2mac_crs,rmii2mac_rx_dv,rmii2mac_rx_er,rmii2mac_rxd[3:0],phy2rmii_crs_dv,phy2rmii_rx_er,phy2rmii_rxd[1:0],rmii2phy_txd[1:0],rmii2phy_tx_en";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "mii_to_rmii,Vivado 2019.1";
begin
end;
