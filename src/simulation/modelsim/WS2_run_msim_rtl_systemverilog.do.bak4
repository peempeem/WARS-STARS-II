transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -sv -work work +incdir+C:/Users/James/LOCAL/ece385/final/WARS-STARS-II/src {C:/Users/James/LOCAL/ece385/final/WARS-STARS-II/src/Blitter.sv}
vlib soc
vmap soc soc

vlog -sv -work work +incdir+C:/Users/James/LOCAL/ece385/final/WARS-STARS-II/src {C:/Users/James/LOCAL/ece385/final/WARS-STARS-II/src/testbench.sv}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L fiftyfivenm_ver -L rtl_work -L work -L soc -voptargs="+acc"  testbench

add wave *
view structure
view signals
run 2000 ns
